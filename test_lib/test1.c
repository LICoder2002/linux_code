#include "head1.h"

void print(const char* msg)
{
    printf("%s : %lld\n", msg, (long long)time(NULL));
}
