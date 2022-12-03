#pragma once

#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <cassert>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEBUG 0
#define NOTICE 1
#define WARINING 2
#define FATAL 3
#define LOGFILE "serverTcp.log"

const char *log_level[] = {"DEBUG", "NOTICE", "WARINING", "FATAL"};

void logMsg(int level, const char *fomat, ...)
{
    assert(level >= DEBUG && level <= FATAL);

    char *name = getenv("USER");

    char logInfo[1024];

    va_list ap; // ap->char*

    va_start(ap, fomat);

    vsnprintf(logInfo, sizeof(logInfo) - 1, fomat, ap);

    va_end(ap);

    umask(0);
    int fd = open(LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0666);

    FILE *out = (level == FATAL) ? stderr : stdout;

    fprintf(out, "%s | %u | %s | %s\n", log_level[level], (unsigned long)time(nullptr), name == nullptr ? "unknow" :name, logInfo);
}

void logMessage(int level, const char *format, ...)
{
    assert(level >= DEBUG);
    assert(level <= FATAL);

    char *name = getenv("USER");

    char logInfo[1024];
    va_list ap; // ap -> char*
    va_start(ap, format);

    vsnprintf(logInfo, sizeof(logInfo) - 1, format, ap);

    va_end(ap); // ap = NULL

    // 每次打开太麻烦
    umask(0);
    int fd = open(LOGFILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    assert(fd >= 0);

    FILE *out = (level == FATAL) ? stderr : stdout;

    dup2(fd, 1);
    dup2(fd, 2);

    fprintf(out, "%s | %u | %s | %s\n",
            log_level[level],
            (unsigned int)time(nullptr),
            name == nullptr ? "unknow" : name,
            logInfo);

    fflush(out); // 将C缓冲区中的数据刷新到OS
    fsync(fd);   // 将OS中的数据尽快刷盘 

    close(fd);
    // char *s = format;
    // while(s){
    //     case '%':
    //         if(*(s+1) == 'd')  int x = va_arg(ap, int);
    //     break;
    // }
}