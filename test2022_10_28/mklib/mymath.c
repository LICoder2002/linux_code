#include "mymath.h"

int addToVal(int from, int to) 
{
    assert(from <= to);
    int res = 0;
    for(int i = from; i <= to; ++i) 
    {
        res += i;
    }
    return res;
}
