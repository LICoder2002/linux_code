#pragma once

#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define IPC_PATH "./.fifo"