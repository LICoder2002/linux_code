#include <iostream>
#include <string>
#include <functional>
#include <pthread.h>
#include "Log.hpp"

class Task
{
    // typedef std::function<void (int, std::string, uint16_t)> callback_t;
    using callback_t = std::function<void(int, std::string, uint16_t)>;

public:
    Task()
        : _sock(-1), _port(-1) {}

    Task(int sock, const std::string &ip, uint16_t port, callback_t func)
        : _sock(sock), _ip(ip), _port(port), _func(func) {}

    ~Task() {}

    void operator()()
    {
        logMsg(DEBUG, "线程ID[%p]处理%s:%d的请求开始了...", pthread_self(), _ip.c_str(), _port);

        _func(_sock, _ip, _port);

        logMsg(DEBUG, "线程ID[%p]处理%s:%d的请求结束了...", pthread_self(), _ip.c_str(), _port);
    }

private:
    int _sock;        //给用户提供IO1服务的sock
    std::string _ip;  // client ip
    uint16_t _port;   // client port
    callback_t _func; //处理任务的回调方法
};
