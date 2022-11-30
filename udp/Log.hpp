#pragma once

#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <cassert>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#define DEBUG 0
#define NOTICE 1
#define WARINING 2
#define FATAL 3

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

    FILE *out = (level == FATAL) ? stderr : stdout;

    fprintf(out, "%s | %u | %s | %s\n", log_level[level], (unsigned long)time(nullptr), name == nullptr ? "unknow" :name, logInfo);
}

void logMessage(int level, const char *fomat, ...)
{
    assert(level >= DEBUG && level <= FATAL);

    char *name = getenv("USER");

    char logInfo[1024];

    va_list ap; // ap->char*

    va_start(ap, fomat);

    vsnprintf(logInfo, sizeof(logInfo) - 1, fomat, ap);

    va_end(ap);

    FILE *out = (level == FATAL) ? stderr : stdout;

    fprintf(out, "%s | %u | %s | %s\n", log_level[level], (unsigned long)time(nullptr), name == nullptr ? "unknow" :name, logInfo);
}