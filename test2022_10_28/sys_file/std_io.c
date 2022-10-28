#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
 

int main()
{
    // 先验证0,1,2就是标准IO
    char buffer[1024];
    ssize_t s = read(0, buffer, sizeof(buffer)-1);
    if(s > 0)
    {
        buffer[s] = '\0';

        printf("echo: %s", buffer);
    }
    const char *str = "hello write\n";
    int len = strlen(str);

    write(1, str, len);
    write(2, str, len);
   
    
    // 验证012和stdin，stdout，stderr的对应关系
    printf("stdin: %d\n", stdin->_fileno);
    printf("stdout: %d\n", stdout->_fileno);
    printf("stderr: %d\n", stderr->_fileno);

    return 0;
}
