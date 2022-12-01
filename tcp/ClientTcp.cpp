#include "util.hpp"

volatile bool quit = false;

static void Usage(std::string proc)
{
    std::cerr << "Usage:\n\t" << proc << " serverIp serverPort" << std::endl;
    std::cerr << "Example:\n\t" << proc << " 127.0.0.1 8081\n"
              << std::endl;
}
// ./clientTcp serverIp serverPort
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    std::string serverIp = argv[1];
    uint16_t serverPort = atoi(argv[2]);

    // 1. 创建socket SOCK_STREAM
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket: " << strerror(errno) << std::endl;
        exit(SOCKET_ERR);
    }

    // 2. connect，发起链接请求，你想谁发起请求呢？？当然是向服务器发起请求喽
    // 2.1 先填充需要连接的远端主机的基本信息
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverPort);
    inet_aton(serverIp.c_str(), &server.sin_addr);
    // 2.2 发起请求，connect 会自动帮我们进行bind！
    if (connect(sock, (const struct sockaddr *)&server, sizeof(server)) != 0)
    {
        std::cerr << "connect: " << strerror(errno) << std::endl;
        exit(CONN_ERR);
    }
    std::cout << "info : connect success: " << sock << std::endl;

    std::string message;
    while (!quit)
    {
        message.clear();
        std::cout << "请输入你的消息# ";
        std::getline(std::cin, message);
        if (strcasecmp(message.c_str(), "quit") == 0)
            quit = true;

        ssize_t s = write(sock, message.c_str(), message.size());
        if (s > 0)
        {
            message.resize(1024);
            ssize_t s = read(sock, (char *)(message.c_str()), 1024);
            if (s > 0)
                message[s] = 0;
            std::cout << "Server Echo--> " << message << std::endl;
        }
        else if (s <= 0)
        {
            break;
        }
    }
    close(sock);
    return 0;
}