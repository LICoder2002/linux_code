#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<assert.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<ctype.h>



//#define NUM 1024
//#define SIZE 128
//
//char command_line[NUM];
//char* command_args[SIZE];
//
//int main()
//{
//    while(1)
//    {
//        //1. 显示提示符
//        printf("[张三@localhost ~]$ ");
//
//        fflush(stdout);
//
//        //2. 获取用户输入
//        
//        memset(command_line, '\0', sizeof(command_line)*sizeof(char));
//
//        fgets(command_line, NUM, stdin); //键盘：标准输入 stdin 获取c字符串 以'\0'结尾，缓冲区中存在'\n'
//        command_line[strlen(command_line)-1] = '\0'; //替换'\n'
//
//        //3.字符串切分
//        command_args[0] = strtok(command_line, SEP);
//        // strtok 截取成功，返回字符串起始地址
//        // 截取失败，返回NULL
//        int index = 1;
//        while(command_args[index++] = strtok(NULL, SEP));
//
//      
//
//        // 5. 创建进程,执行
//        pid_t id = fork();
//        if(id == 0)
//        {
//           execvp(command_args[0], command_args);
//           exit(1);
//        }
//
//                int status = 0;
//        pid_t ret = waitpid(id, &status, 0);
//        if(ret > 0)
//        {
//            printf("等待子进程成功: sig: %d, code: %d\n", status&0x7F, (status>>8)&0xFF);
//        }
//
//        
//    }
//    return 0;
//}
//
#define SEP " "
#define NUM 1024
#define SIZE 128

#define DROP_SPACE(s) do { while(isspace(*s)) ++s;} while(0)

#define NONE_REDIR -1
#define INPUT_REDIR 0 //输入重定向 
#define OUTPUT_REDIR 1 //输出重定向 
#define APPEND_REDIR 2 //追加重定向

int g_redir_flag = NONE_REDIR;
char* g_redir_filename = NULL;


char command_line[NUM];
char *command_args[SIZE];

extern char **environ;
char env_buffer[NUM];




//对应上层的内建命令
int ChangeDir(const char* new_path)
{
    chdir(new_path);
    return 0; //调用成功
}

void PutEnvInMyShell(char* new_env)
{
    putenv(new_env);
}

void CheckDir(char* commands)
{
    assert(commands);
    char* start = commands ;
    char* end = commands  + strlen(commands) ;

    while(start < end) 
    {
        if(*start == '>')
        {
            if(*(start + 1) == '>')
            {
                *start = '\0';
                start += 2;
                g_redir_flag = APPEND_REDIR;
                DROP_SPACE(start);
                g_redir_filename = start;
                break;
            }
            else
            {
                *start = '\0';
                start++;
                g_redir_flag = OUTPUT_REDIR;
                DROP_SPACE(start);
                g_redir_filename = start;
                break;
            }
        }
        else if(*start == '<') 
        {
            *start = '\0';
            start++;
            DROP_SPACE(start);
            g_redir_flag = INPUT_REDIR;
            g_redir_filename = start;
        }
        else
        {
            ++start;
        }
    }
}

int main()
{
    //shell 本质上就是一个死循环
    while(1)
    {
        g_redir_flag = NONE_REDIR;
        g_redir_filename = NULL;

        //不关心获取这些属性的接口, 搜索一下
        //1. 显示提示符
        printf("[张三@我的主机名 当前目录]# ");
        fflush(stdout);
        //2. 获取用户输入
        memset(command_line, '\0', sizeof(command_line)*sizeof(char));
        fgets(command_line, NUM, stdin); //键盘，标准输入，stdin, 获取到的是c风格的字符串, '\0'
        command_line[strlen(command_line) - 1] = '\0';// 清空\n
       
        // ls -a -l>log.txt     ls -a -l >> log.txt  ...
        CheckDir(command_line);
        

        //3. "ls -a -l -i" -> "ls" "-a" "-l" "-i" 字符串切分
        command_args[0] = strtok(command_line, SEP);
        int index = 1;

        //给ls命令添加颜色
        if(strcmp(command_args[0], "ls") == 0)
            command_args[index++] =(char*) "--color=auto";

        // = 是故意这么写的
        // strtok 截取成功，返回字符串其实地址
        // 截取失败，返回NULL
        while(command_args[index++] = strtok(NULL, SEP));

        //for debug
        //for(int i = 0 ; i < index; i++)
        //{
        //    printf("%d : %s\n", i, command_args[i]);
        //}
        // 4. TODO, 编写后面的逻辑, 内建命令
        

        if(strcmp(command_args[0], "cd") == 0)
        {
            ChangeDir(command_args[1]); //让调用方进行路径切换，即父进程
            continue;
        }
        


        if(strcmp(command_args[0], "cd") == 0)
        {
            //目前环境变量信息在command_line, 会被清空
            //需要自己保存
            strcpy(env_buffer, command_args[1]);
            PutEnvInMyShell(env_buffer);
            continue;
        }



        //
        // 5. 创建进程,执行
        pid_t id = fork();
        if(id == 0)
        {
            int fd = -1;
            switch(g_redir_flag)
            {
                case NONE_REDIR:
                    break;
                case INPUT_REDIR:
                    fd = open(g_redir_filename, O_RDONLY);
                    dup2(fd, 0);
                    break;
                case OUTPUT_REDIR:
                    fd = open(g_redir_filename, O_WRONLY | O_CREAT | O_TRUNC);
                    dup2(fd, 1);
                    break;
                case APPEND_REDIR:
                    fd = open(g_redir_filename, O_WRONLY | O_CREAT | O_APPEND);
                    dup2(fd, 1);
                    break;

                default:
                    printf("BUG?\n");
                    break;
            }

            //child
            // 6. 程序替换
            //exec*?
            execvp(command_args[0]/*不就是保存的是我们要执行的程序名字吗？*/, command_args);
            exit(1); //执行到这里，子进程一定替换失败
        }
        int status = 0;
        pid_t ret = waitpid(id, &status, 0);
        if(ret > 0)
        {
            printf("等待子进程成功: sig: %d, code: %d\n", status&0x7F, (status>>8)&0xFF);
        }
    }// end while
    return 0;
}
