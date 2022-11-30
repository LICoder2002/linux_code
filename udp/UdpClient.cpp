#include <iostream>
#include <string>
#include <unordered_map>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "Log.hpp"

struct sockaddr_in server;

static void Usage(const std::string name)
{
    std::cout << "Usage:\n\t" << name << " server_ip server_port " << std::endl;
}

void *handler(void *args)
{
    while (true)
    {
        int sockFd = *(int *)args;
        char buffer[1024];

                    struct sockaddr_in tmp;      //输出型参数
            socklen_t len = sizeof(tmp); //输入输出型参数

            ssize_t s = recvfrom(sockFd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&tmp, &len);
            if (s > 0)
            {
                buffer[s] = 0;
                std::cout << "server echo# " << buffer << std::endl;
            }

    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(3);
    }

    // 1、根据命令行，设置服务器的ip和port
    std::string server_ip = argv[1];
    uint16_t server_port = atoi(argv[2]);

    // 2、创建客户端
    // 2.1 创建socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(sockfd > 0);

    // 2.1 填写服务器对应信息
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(server_ip.c_str());
    server.sin_port = htons(server_port);

    //创建子线程接收sever回复的消息
    pthread_t tid;
    pthread_create(&tid, nullptr, handler, (void *)&sockfd);

    // 3、通信
    std::string buffer;
    while (true)
    {
        std::cerr << "Please Enter# ";
        std::getline(std::cin, buffer);

        //发送信息给server
        if (sendto(sockfd, buffer.c_str(), buffer.size(), 0, (const struct sockaddr *)&server, sizeof(server)) == -1)
        {
            logMsg(WARINING, "client send massage to server: %s, sockFd: %d", strerror(errno), sockfd);
        }
    }

    return 0;
}