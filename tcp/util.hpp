#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Log.hpp"

#define SOCKET_ERR 1 //创建socket错误
#define BIND_ERR 2   // bind错误
#define LISTEN_ERR 3 //监听错误
#define USAGE_ERR 4  //使用命令行参数错误
#define CONN_ERR 5   //连接错误

#define BUFFER_SIZE 1024 //缓冲区默认大小