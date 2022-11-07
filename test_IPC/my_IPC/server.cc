#include "comm.h"

using namespace std;

int main()
{
    //获取key
    key_t key = ftok(PATH_NAME, PROJ_ID);
    if (key < 0)
    {
        cerr << "ftok error" << endl;
        exit(1);
    }

    
    //创建共享内存
    cout << "开始创建共享内存..." << endl;
    sleep(2);
    int shmid = shmget(key, SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid < 0)
    {
        cerr << "shmget error" << endl;
        exit(2);
    }
    cout << "创建共享内存成功" << endl;

    printf("key = %x\n", key);
    printf("shmid = %d\n", shmid);

    //关联共享内存
    cout << "开始关联共享内存..." << endl;
    char *str = (char *)shmat(shmid, nullptr, 0);
    if (str == (char *)-1)
    {
        cerr << "shmat error" << endl;
        exit(3);
    }
    cout << "关联共享内存成功" << endl;

    
    //使用共享内存
    cout << "开始使用共享内存..." << endl;
    sleep(2);
    while(true)
    {
        // 让读端进行等待
        printf("%s\n", str);
        sleep(1);
    }
    cout << "使用共享内存结束" << endl;

    cout << "开始去关联共享内存..." << endl;
    sleep(2);
    shmdt(str);
    cout << "去关联共享内存成功" << endl;


    cout << "开始释放共享内存..." << endl;
    sleep(2);
    shmctl(shmid, IPC_RMID, nullptr);
    cout << "释放共享内存成功" << endl;

    return 0;

}