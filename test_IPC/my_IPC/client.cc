#include "comm.h"

using namespace std;

int main()
{
    //获取相同的key
    key_t key = ftok(PATH_NAME, PROJ_ID);
    if (key < 0)
    {
        cerr << "ftok error" << endl;
        exit(1);
    }

    // 获取共享内存
    int shmid = shmget(key, SIZE, IPC_CREAT);
    if (shmid < 0)
    {
        cerr << "shmget error" << endl;
        exit(2);
    }

    //关联共享内存
    char *str = (char *)shmat(shmid, nullptr, 0);
    if (str == (char *)-1)
    {
        cerr << "shmat error" << endl;
        exit(3);
    }

    //使用共享内存
    int cnt = 0;
    while (cnt <= 26)
    {
        str[cnt] = 'A' + cnt;
        ++cnt;
        str[cnt] = '\0';
        sleep(1);
    }

    // 去关联
    shmdt(str);
    return 0;
}