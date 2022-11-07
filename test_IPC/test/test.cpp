#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

using namespace std;

#define PATH_NAME "/home/lhf/linux_code/test_IPC" //路径名
#define PROJ_ID 0x14                              //整数标识符
#define SIZE 4096                                 //共享内存的大小

int main()
{
    key_t key = ftok(PATH_NAME, PROJ_ID); //获取key值
    if (key < 0)
    {
        cerr << "ftok error" << endl;
        return 1;
    }

    int shm = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666); //创建新的共享内存
    if (shm < 0)
    {
        cerr << "shmget error" << endl;
        return 2;
    }

    cout << "key = " << key << endl;
    cout << "shm = " << shm << endl;

    cout << "开始关联共享内存..."<< endl;
    sleep(2);
    char* str = (char*)shmat(shm, nullptr, 0); 
    if(str == (char*) -1)
    {
        cerr << "shmat error" << endl;
        return 1;
    }

    cout << "关联共享内存成功" << endl;

    cout << "开始去关联..." << endl;
    sleep(2);
    shmdt(str);
    cout << "去关联成功" << endl;

    cout << "开始释放共享内存..." << endl;
    sleep(2);
    shmctl(shm, IPC_RMID, nullptr);
    cout << "释放共享内存成功" << endl;

    return 0;
}
