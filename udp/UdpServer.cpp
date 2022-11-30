
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Log.hpp"

static void Usage(const std::string proc)
{
    std::cout << "Usage:\n\t" << proc << " port [ip]" << std::endl;
}

class UdpServer
{
public:
    UdpServer(int port, std::string ip = "")
        : _port((uint16_t)port),
          _ip(ip),
          _sockFd(-1)
    {
    }

    ~UdpServer() {}

public:
    void init()
    {
        // 1. 创建socket套接字
        _sockFd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_sockFd < 0)
        {
            logMsg(FATAL, "socket: %s, sockFd: %d", strerror(errno), _sockFd);
            exit(1);
        }
        logMsg(DEBUG, "socket create success, sockFd: %d", _sockFd);

        // 2. 绑定网络信息
        // 2.1 填充基本信息到 struct sockaddr_in
        struct sockaddr_in local;
        bzero(&local, sizeof(local)); // memset
        //填充协议家族，域
        local.sin_family = AF_INET;
        //填写端口，会发送给对方，_port会发送到网络
        local.sin_port = htons(_port);

        // 服务器都必须具有IP地址,"xx.yy.zz.aaa",字符串风格点分十进制 -> 4字节IP -> uint32_t ip
        // INADDR_ANY(0): 程序员不关心会bind到哪一个ip， 任意地址bind，强烈推荐的做法，所有服务器一般的做法
        // inet_addr: 指定填充确定的IP，特殊用途，或者测试时使用，除了做转化，还会自动给我们进行 h—>n
        local.sin_addr.s_addr = _ip.empty() ? htonl(INADDR_ANY) : inet_addr(_ip.c_str());

        // 2.2 绑定bind 网络信息
        if (bind(_sockFd, (const struct sockaddr *)&local, sizeof(local)) == -1)
        {
            logMsg(FATAL, "bind: %s, sockFd: %d", strerror(errno), _sockFd);
            exit(2);
        }
        logMsg(DEBUG, "socket bind success, sockFd: %d", _sockFd);
    }

    void init2()
    {
        // 1. 创建socket套接字
        _sockFd = socket(AF_INET, SOCK_DGRAM, 0); // 就是打开了一个文件
        if (_sockFd < 0)
        {
            logMessage(FATAL, "socket:%s:%d", strerror(errno), _sockFd);
            exit(1);
        }
        logMessage(DEBUG, "socket create success: %d", _sockFd);
        // 2. 绑定网络信息，指明ip+port
        // 2.1 先填充基本信息到 struct sockaddr_in
        struct sockaddr_in local;     // local在哪里开辟的空间? 用户栈 -> 临时变量 -> 写入内核中
        bzero(&local, sizeof(local)); // memset
        // 填充协议家族，域
        local.sin_family = AF_INET;
        // 填充服务器对应的端口号信息，一定是会发给对方的，port_一定会到网络中
        local.sin_port = htons(_port);
        // 服务器都必须具有IP地址,"xx.yy.zz.aaa",字符串风格点分十进制 -> 4字节IP -> uint32_t ip
        // INADDR_ANY(0): 程序员不关心会bind到哪一个ip， 任意地址bind，强烈推荐的做法，所有服务器一般的做法
        // inet_addr: 指定填充确定的IP，特殊用途，或者测试时使用，除了做转化，还会自动给我们进行 h—>n
        local.sin_addr.s_addr = _ip.empty() ? htonl(INADDR_ANY) : inet_addr(_ip.c_str());
        // 2.2 bind 网络信息
        if (bind(_sockFd, (const struct sockaddr *)&local, sizeof(local)) == -1)
        {
            logMessage(FATAL, "bind: %s:%d", strerror(errno), _sockFd);
            exit(2);
        }
        logMessage(DEBUG, "socket bind success: %d", _sockFd);
        // done
    }

    void start()
    {
        char inbuffer[1024];  //存放读取的数据
        char outbuffer[1024]; //存放发送的数据

        while (true)
        {
            struct sockaddr_in peer;      //输出型参数
            socklen_t len = sizeof(peer); //输入输出型参数

            ssize_t s = recvfrom(_sockFd, inbuffer, sizeof(inbuffer) - 1, 0, (struct sockaddr *)&peer, &len);
            if (s > 0)
            {
                inbuffer[s] = 0;
            }
            else if (s == -1)
            {
                logMsg(WARINING, "recvform: %s, sockFd: %d", strerror(errno), _sockFd);
                continue;
            }

            //读取成功
            //对方的IP地址
            std::string peerIp = inet_ntoa(peer.sin_addr);
            //对方的端口号
            uint32_t peerPort = ntohs(peer.sin_port);

            checkOnlineUsers(peerIp, peerPort, peer);

            //客户端发送的信息
            logMsg(NOTICE, "【%s:%d】# %s", peerIp.c_str(), peerPort, inbuffer);

            messageRoute(peerIp, peerPort, inbuffer);
        }
    }

    void checkOnlineUsers(std::string &ip, uint32_t port, struct sockaddr_in &peer)
    {
        std::string key = ip;
        key += ":";
        key += std::to_string(port);

        auto iter = _users.find(key);
        if (iter == _users.end())
        {
            _users.insert({key, peer});
        }
        else
        {
            //...
        }
    }

    void messageRoute(std::string ip, uint32_t port, std::string info)
    {
        std::string message = "【";
        message += ip;
        message += ":";
        message += std::to_string(port);
        message += "】# ";
        message += info;

        for(auto& user : _users)
        {
            sendto(_sockFd, message.c_str(), message.size(), 0, (const sockaddr*)&user.second, sizeof(user.second));
        }
    }

private:
    //服务器端口
    uint16_t _port;
    //服务器ip地址
    std::string _ip;
    //服务器socket fd信息
    int _sockFd;

    //在线用户
    std::unordered_map<std::string, struct sockaddr_in> _users;
};

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3)
    {
        Usage(argv[0]);
        exit(3);
    }

    uint16_t port = atoi(argv[1]);
    std::string ip;

    if (argc == 3)
    {
        ip = argv[2];
    }

    UdpServer udpServer(port, ip);

    udpServer.init();
    udpServer.start();

    return 0;
}

/*
#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <unordered_map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Log.hpp"

static void Usage(const std::string porc)
{
    std::cout << "Usage:\n\t" << porc << " port [ip]" << std::endl;
}

/// @brief  我们想写一个简单的udpSever
/// 云服务器有一些特殊情况：
/// 1. 禁止你bind云服务器上的任何确定IP， 只能使用INADDR_ANY，如果你是虚拟机，随意
class UdpServer
{
public:
    UdpServer(int port, std::string ip = "") : port_((uint16_t)port), ip_(ip), sockfd_(-1)
    {
    }
    ~UdpServer()
    {
    }

public:
    void init()
    {
        // 1. 创建socket套接字
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0); // 就是打开了一个文件
        if (sockfd_ < 0)
        {
            logMessage(FATAL, "socket:%s:%d", strerror(errno), sockfd_);
            exit(1);
        }
        logMessage(DEBUG, "socket create success: %d", sockfd_);
        // 2. 绑定网络信息，指明ip+port
        // 2.1 先填充基本信息到 struct sockaddr_in
        struct sockaddr_in local;     // local在哪里开辟的空间? 用户栈 -> 临时变量 -> 写入内核中
        bzero(&local, sizeof(local)); // memset
        // 填充协议家族，域
        local.sin_family = AF_INET;
        // 填充服务器对应的端口号信息，一定是会发给对方的，port_一定会到网络中
        local.sin_port = htons(port_);
        // 服务器都必须具有IP地址,"xx.yy.zz.aaa",字符串风格点分十进制 -> 4字节IP -> uint32_t ip
        // INADDR_ANY(0): 程序员不关心会bind到哪一个ip， 任意地址bind，强烈推荐的做法，所有服务器一般的做法
        // inet_addr: 指定填充确定的IP，特殊用途，或者测试时使用，除了做转化，还会自动给我们进行 h—>n
        local.sin_addr.s_addr = ip_.empty() ? htonl(INADDR_ANY) : inet_addr(ip_.c_str());
        // 2.2 bind 网络信息
        if (bind(sockfd_, (const struct sockaddr *)&local, sizeof(local)) == -1)
        {
            logMessage(FATAL, "bind: %s:%d", strerror(errno), sockfd_);
            exit(2);
        }
        logMessage(DEBUG, "socket bind success: %d", sockfd_);
        // done
    }

    void start()
    {
        // 服务器设计的时候，服务器都是死循环
        char inbuffer[1024];  //将来读取到的数据，都放在这里
        char outbuffer[1024]; //将来发送的数据，都放在这里
        while (true)
        {
            struct sockaddr_in peer;      //输出型参数
            socklen_t len = sizeof(peer); //输入输出型参数

            // demo2
            //  UDP无连接的
            //  对方给你发了消息，你想不想给对方回消息？要的！后面的两个参数是输出型参数
            ssize_t s = recvfrom(sockfd_, inbuffer, sizeof(inbuffer) - 1, 0,
                                 (struct sockaddr *)&peer, &len);
            if (s > 0)
            {
                inbuffer[s] = 0; //当做字符串
            }
            else if (s == -1)
            {
                logMessage(WARINING, "recvfrom: %s:%d", strerror(errno), sockfd_);
                continue;
            }
            // 读取成功的,除了读取到对方的数据，你还要读取到对方的网络地址[ip:port]
            std::string peerIp = inet_ntoa(peer.sin_addr);       //拿到了对方的IP
            uint32_t peerPort = ntohs(peer.sin_port); // 拿到了对方的port

            checkOnlineUser(peerIp, peerPort, peer); //如果存在，什么都不做，如果不存在，就添加

            // 打印出来客户端给服务器发送过来的消息
            logMessage(NOTICE, "[%s:%d]# %s", peerIp.c_str(), peerPort, inbuffer);

            // for(int i = 0; i < strlen(inbuffer); i++)
            // {
            //     if(isalpha(inbuffer[i]) && islower(inbuffer[i])) outbuffer[i] = toupper(inbuffer[i]);
            //     else outbuffer[i] = toupper(inbuffer[i]);
            // }
            messageRoute(peerIp, peerPort,inbuffer); //消息路由

            // 线程池！

            // sendto(sockfd_, outbuffer, strlen(outbuffer), 0, (struct sockaddr*)&peer, len);

            // demo1
            // logMessage(NOTICE, "server 提供 service 中....");
            // sleep(1);
        }
    }

    void checkOnlineUser(std::string &ip, uint32_t port, struct sockaddr_in &peer)
    {
        std::string key = ip;
        key += ":";
        key += std::to_string(port);
        auto iter = users.find(key);
        if(iter == users.end())
        {
            users.insert({key, peer});
        }
        else
        {
            // iter->first, iter->second->
            // do nothing
        }
    }

    void messageRoute(std::string ip, uint32_t port, std::string info)
    {

        std::string message = "[";
        message += ip;
        message += ":";
        message += std::to_string(port);
        message += "]# ";
        message += info;

        for(auto &user : users)
        {
            sendto(sockfd_, message.c_str(), message.size(), 0, (struct sockaddr*)&(user.second), sizeof(user.second));
        }
    }

private:
    // 服务器必须得有端口号信息
    uint16_t port_;
    // 服务器必须得有ip地址
    std::string ip_;
    // 服务器的socket fd信息
    int sockfd_;
    // onlineuser
    std::unordered_map<std::string, struct sockaddr_in> users;
};

// struct client{
//     struct sockaddr_in peer;
//     uint64_t when; //peer如果在when之前没有再给我发消息，我就删除这用户
// }

// ./udpServer port [ip]
int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3) //反面：argc == 2 || argc == 3
    {
        Usage(argv[0]);
        exit(3);
    }
    uint16_t port = atoi(argv[1]);
    std::string ip;
    if (argc == 3)
    {
        ip = argv[2];
    }

    UdpServer svr(port, ip);
    svr.init();
    svr.start();

    return 0;
}

*/
