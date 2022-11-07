#pragma once

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PATH_NAME "/home/lhf/linux_code/test_IPC"
#define PROJ_ID 0x666
#define SIZE 4096

/*
key_t createKey()
{
    key_t key = ftok(PATH_NAME, PROJ_ID);
    if(key < 0)
    {
        std::cerr << "ftok error" << std::endl;
        exit(1);
    }
    return key;
}
*/
/*
#define READER O_RDONLY
#define WRITER O_WRONLY

int Open(const std::string &filename, int flags)
{
    return open(filename.c_str(), flags);
}

int Wait(int fd)
{
    uint32_t values = 0;
    ssize_t s = read(fd, &values, sizeof(values));
    return s;
}

int Signal(int fd)
{
    uint32_t cmd = 1;
    write(fd, &cmd, sizeof(cmd));
}

int Close(int fd, const std::string filename)
{
    close(fd);
    unlink(filename.c_str());
}

*/