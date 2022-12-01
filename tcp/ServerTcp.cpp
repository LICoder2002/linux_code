#include <signal.h>
#include <sys/wait.h>
#include <pthread.h>
#include "util.hpp"

class ServerTcp; //申明

class ThreadData
{
public:
    uint16_t _clientPort;
    std::string _clientIp;
    int _sock;
    ServerTcp *_this;

public:
    ThreadData(uint16_t port, const std::string &ip, int sock, ServerTcp *ts)
        : _clientPort(port),
          _clientIp(ip),
          _sock(sock),
          _this(ts)
    {
    }
};

class ServerTcp
{
public:
    ServerTcp(int port, const std::string &ip = "")
        : _port(port),
          _ip(ip),
          _listenSock(-1)
    {
    }

    ~ServerTcp()
    {
    }

public:
    //初始化服务器
    void init()
    {
        // 1. 创建socket
        _listenSock = socket(PF_INET, SOCK_STREAM, 0);
        if (_listenSock < 0)
        {
            logMsg(FATAL, "socket: %s", strerror(errno));
            exit(SOCKET_ERR);
        }
        logMsg(DEBUG, "socket create success: %s, sockFd: %d", strerror(errno), _listenSock);

        // 2. bind绑定
        // 2.1 填写服务器信息
        struct sockaddr_in local; //用户栈
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        // local.sin_addr.s_addr = _ip.empty() ? htonl(INADDR_ANY) : inet_addr(_ip.c_str());
        _ip.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(_ip.c_str(), &local.sin_addr));

        // 2.2 本地绑定socket信息，写入sock_对应的内核区域
        if (bind(_listenSock, (const struct sockaddr *)&local, sizeof(local)) < 0)
        {
            logMsg(FATAL, "bind: %s", strerror(errno));
            exit(BIND_ERR);
        }
        logMsg(DEBUG, "bind success: %s, sockFd: %d", strerror(errno), _listenSock);

        // 3. 监听socket，因为tcp是面向连接的
        if (listen(_listenSock, 5) < 0)
        {
            logMsg(FATAL, "listen: %s", strerror(errno));
            exit(LISTEN_ERR);
        }
        logMsg(DEBUG, "listen success: %s, sockFd: %d", strerror(errno), _listenSock);
    }

    static void *threadRoutine(void *args)
    {
        pthread_detach(pthread_self());//线程分离
        ThreadData* td = static_cast<ThreadData*>(args);
        td->_this->transServer(td->_sock, td->_clientIp, td->_clientPort);
        delete td;
        return nullptr;
    }

    //运行服务器
    void loop()
    {
        // signal(SIGCHLD, SIG_IGN);
        while (true)
        { //客户端
            struct sockaddr_in peer;
            socklen_t len = sizeof peer;

            // 4. 获取与客户端的连接， accept调用成功的返回值是一个新的 socket fd
            int clientSock = accept(_listenSock, (struct sockaddr *)&peer, &len);
            if (clientSock < 0)
            {
                //获取连接失败
                logMsg(FATAL, "accept: %s", strerror(errno));
                continue;
            }

            // 4.1 获取客户端信息
            uint32_t peerPort = ntohs(peer.sin_port);
            std::string peerIp = inet_ntoa(peer.sin_addr);

            logMsg(DEBUG, "accept: %s | %s[%d], socket fd: %d", strerror(errno), peerIp.c_str(), peerPort, clientSock);

            // 5. 提供服务
            // 5.0 v0   -- 单进程，一旦进入transServer，主执行流执行，就无法向其他客户端提供服务，只能等当前服务退出后，才能继续提供服务

            // transServer(clientSock, peerIp, peerPort);

            // 5.1 v1.0 -- 多进程

            /*
            pid_t id = fork();
            assert(id != -1);
            if(id == 0)
            {
                //子进程
                close(_listenSock);
                transServer(clientSock, peerIp, peerPort);

                exit(0);//进入僵尸状态
            }

            //父进程 使用SIGCHLD，不用等待子进程
            close(clientSock); //一定

            */

            // 5.1 v1.1 -- 多进程

            /*

            //爷爷进程
            pid_t id = fork();
            assert(id != -1);
            if(id == 0)
            {
                //爸爸进程
                close(_listenSock);
                if(fork() > 0) exit(0); //爸爸进程退出

                //孙子进程 退出后变成孤儿进程，由操作系统进行管理
                transServer(clientSock, peerIp, peerPort);
                exit(0);
            }

            //爷爷进程
            close(clientSock);
            pid_t res = waitpid(id, nullptr, 0); //阻塞式等待
            assert(res > 0);
            (void)res;

            */

            // 5.2 v2   -- 多线程

            ThreadData *td = new ThreadData(peerPort, peerIp, clientSock, this);

            pthread_t tid;
            pthread_create(&tid, nullptr, threadRoutine, (void *)td);
        }
    }

    //大小写转换
    void transServer(int sock, const std::string &clientIp, uint16_t clientPort)
    {
        assert(sock >= 0);
        assert(clientPort >= 1024);
        assert(!clientIp.empty());

        char inbuffer[1024];
        while (true)
        {
            ssize_t s = read(sock, inbuffer, sizeof(inbuffer) - 1);
            if (s > 0)
            {
                // read success
                inbuffer[s] = '\0';
                if (strcasecmp(inbuffer, "quit") == 0)
                {
                    logMessage(DEBUG, "client quit, %s[%d]", clientIp.c_str(), clientPort);
                    break;
                }
                logMsg(DEBUG, "transform before: %s[%d]--> %s", clientIp.c_str(), clientPort, inbuffer);

                //转换
                for (int i = 0; i < s; ++i)
                {
                    if (isalpha(inbuffer[i]) && islower(inbuffer[i]))
                    {
                        inbuffer[i] = toupper(inbuffer[i]);
                    }
                }
                logMsg(DEBUG, "transform after: %s[%d]--> %s", clientIp.c_str(), clientPort, inbuffer);

                write(sock, inbuffer, strlen(inbuffer));
            }
            else if (s == 0)
            {
                // pipe: 读端一直在读，写端停止写入，并关闭写端，读端会返回0
                // s == 0: 代表client退出
                logMessage(DEBUG, "client quit, %s[%d]", clientIp.c_str(), clientPort);
                break;
            }
            else
            {
                logMsg(DEBUG, "client : %s[%d], read: %s", clientIp.c_str(), clientPort, strerror(errno));
                break;
            }
        }
        close(sock);
        logMsg(DEBUG, "server close %d done", sock);
    }

private:
    // socket
    int _listenSock;
    //端口号
    uint16_t _port;
    // ip地址
    std::string _ip;
};

static void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port ip" << std::endl;
    std::cerr << "example:\n\t" << proc << " 8080 127.0.0.1\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }

    uint16_t port = atoi(argv[1]);
    std::string ip;
    if (argc == 3)
    {
        ip = argv[2];
    }
    ServerTcp server(port, ip);
    server.init();
    server.loop();

    return 0;
}

/*
#include "util.hpp"
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

class ServerTcp; // 申明一下ServerTcp

class ThreadData
{
public:
    uint16_t clientPort_;
    std::string clinetIp_;
    int sock_;
    ServerTcp *this_;
public:
    ThreadData(uint16_t port, std::string ip, int sock,  ServerTcp *ts)
        : clientPort_(port), clinetIp_(ip), sock_(sock),this_(ts)
    {}
};

class ServerTcp
{
public:
    ServerTcp(uint16_t port, const std::string &ip = "") : port_(port), ip_(ip), listenSock_(-1)
    {
    }
    ~ServerTcp()
    {
    }

public:
    void init()
    {
        // 1. 创建socket
        listenSock_ = socket(PF_INET, SOCK_STREAM, 0);
        if (listenSock_ < 0)
        {
            logMessage(FATAL, "socket: %s", strerror(errno));
            exit(SOCKET_ERR);
        }
        logMessage(DEBUG, "socket: %s, %d", strerror(errno), listenSock_);

        // 2. bind绑定
        // 2.1 填充服务器信息
        struct sockaddr_in local; // 用户栈
        memset(&local, 0, sizeof local);
        local.sin_family = PF_INET;
        local.sin_port = htons(port_);
        ip_.empty() ? (local.sin_addr.s_addr = INADDR_ANY) : (inet_aton(ip_.c_str(), &local.sin_addr));
        // 2.2 本地socket信息，写入sock_对应的内核区域
        if (bind(listenSock_, (const struct sockaddr *)&local, sizeof local) < 0)
        {
            logMessage(FATAL, "bind: %s", strerror(errno));
            exit(BIND_ERR);
        }
        logMessage(DEBUG, "bind: %s, %d", strerror(errno), listenSock_);

        // 3. 监听socket，为何要监听呢？tcp是面向连接的！
        if (listen(listenSock_, 5 ) < 0)
        {
            logMessage(FATAL, "listen: %s", strerror(errno));
            exit(LISTEN_ERR);
        }
        logMessage(DEBUG, "listen: %s, %d", strerror(errno), listenSock_);
        // 运行别人来连接你了
    }
    static void *threadRoutine(void *args)
    {
        pthread_detach(pthread_self()); //设置线程分离
        ThreadData *td = static_cast<ThreadData*>(args);
        td->this_->transService(td->sock_, td->clinetIp_, td->clientPort_);
        delete td;
        return nullptr;
    }
    void loop()
    {
        // signal(SIGCHLD, SIG_IGN); // only Linux

        while (true)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);
            // 4. 获取连接, accept 的返回值是一个新的socket fd ？？
            int serviceSock = accept(listenSock_, (struct sockaddr *)&peer, &len);
            if (serviceSock < 0)
            {
                // 获取链接失败
                logMessage(WARINING, "accept: %s[%d]", strerror(errno), serviceSock);
                continue;
            }
            // 4.1 获取客户端基本信息
            uint16_t peerPort = ntohs(peer.sin_port);
            std::string peerIp = inet_ntoa(peer.sin_addr);

            logMessage(DEBUG, "accept: %s | %s[%d], socket fd: %d",
                       strerror(errno), peerIp.c_str(), peerPort, serviceSock);
            // 5 提供服务, echo -> 小写 -> 大写
            // 5.0 v0 版本 -- 单进程 -- 一旦进入transService，主执行流，就无法进行向后执行，只能提供完毕服务之后才能进行accept
            // transService(serviceSock, peerIp, peerPort);

            // 5.1 v1 版本 -- 多进程版本 -- 父进程打开的文件会被子进程继承吗？会的
            // pid_t id = fork();
            // assert(id != -1);
            // if(id == 0)
            // {
            //     close(listenSock_); //建议
            //     //子进程
            //     transService(serviceSock, peerIp, peerPort);
            //     exit(0); // 进入僵尸
            // }
            // // 父进程
            // close(serviceSock); //这一步是一定要做的！

            // 5.1 v1.1 版本 -- 多进程版本  -- 也是可以的
            // 爷爷进程
            // pid_t id = fork();
            // if(id == 0)
            // {
            //     // 爸爸进程
            //     close(listenSock_);//建议
            //     // 又进行了一次fork，让 爸爸进程
            //     if(fork() > 0) exit(0);
            //     // 孙子进程 -- 就没有爸爸 -- 孤儿进程 -- 被系统领养 -- 回收问题就交给了系统来回收
            //     transService(serviceSock, peerIp, peerPort);
            //     exit(0);
            // }
            // // 父进程
            // close(serviceSock); //这一步是一定要做的！
            // // 爸爸进程直接终止，立马得到退出码，释放僵尸进程状态
            // pid_t ret = waitpid(id, nullptr, 0); //就用阻塞式
            // assert(ret > 0);
            // (void)ret;

            // 5.2 v2 版本 -- 多线程
            // 这里不需要进行关闭文件描述符吗？？不需要啦
            // 多线程是会共享文件描述符表的！
            ThreadData *td = new ThreadData(peerPort, peerIp, serviceSock, this);
            pthread_t tid;
            pthread_create(&tid, nullptr, threadRoutine, (void*)td);

            // waitpid(); 默认是阻塞等待！WNOHANG
            // 方案1

            // logMessage(DEBUG, "server 提供 service start ...");
            // sleep(1);
        }
    }
    // 大小写转化服务
    // TCP && UDP: 支持全双工
    void transService(int sock, const std::string &clientIp, uint16_t clientPort)
    {
        assert(sock >= 0);
        assert(!clientIp.empty());
        assert(clientPort >= 1024);

        char inbuffer[BUFFER_SIZE];
        while (true)
        {
            ssize_t s = read(sock, inbuffer, sizeof(inbuffer) - 1); //我们认为我们读到的都是字符串
            if (s > 0)
            {
                // read success
                inbuffer[s] = '\0';
                if(strcasecmp(inbuffer, "quit") == 0)
                {
                    logMessage(DEBUG, "client quit -- %s[%d]", clientIp.c_str(), clientPort);
                    break;
                }
                logMessage(DEBUG, "trans before: %s[%d]>>> %s", clientIp.c_str(), clientPort, inbuffer);
                // 可以进行大小写转化了
                for(int i = 0; i < s; i++)
                {
                    if(isalpha(inbuffer[i]) && islower(inbuffer[i]))
                        inbuffer[i] = toupper(inbuffer[i]);
                }
                logMessage(DEBUG, "trans after: %s[%d]>>> %s", clientIp.c_str(), clientPort, inbuffer);

                write(sock, inbuffer, strlen(inbuffer));
            }
            else if (s == 0)
            {
                // pipe: 读端一直在读，写端不写了，并且关闭了写端，读端会如何？s == 0，代表对端关闭
                // s == 0: 代表对方关闭,client 退出
                logMessage(DEBUG, "client quit -- %s[%d]", clientIp.c_str(), clientPort);
                break;
            }
            else
            {
                logMessage(DEBUG, "%s[%d] - read: %s", clientIp.c_str(), clientPort, strerror(errno));
                break;
            }
        }

        // 只要走到这里，一定是client退出了，服务到此结束
        close(sock); // 如果一个进程对应的文件fd，打开了没有被归还，文件描述符泄漏！
        logMessage(DEBUG, "server close %d done", sock);
    }

private:
    // sock
    int listenSock_;
    // port
    uint16_t port_;
    // ip
    std::string ip_;
};

static void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " port ip" << std::endl;
    std::cerr << "example:\n\t" << proc << " 8080 127.0.0.1\n" << std::endl;

}

// ./ServerTcp local_port local_ip
int main(int argc, char *argv[])
{
    if(argc != 2 && argc != 3 )
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    std::string ip;
    if(argc == 3) ip = argv[2];

    ServerTcp svr(port, ip);
    svr.init();
    svr.loop();
    return 0;
}

*/