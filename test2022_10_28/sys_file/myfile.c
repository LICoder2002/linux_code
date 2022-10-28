#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
//
//int main()
//{
//    umask(0);
//    int fd = open("myfile.txt", O_WRONLY | O_CREAT, 0664);
//    if(fd < 0)
//    {
//        perror("open");
//        return 1;
//    }
//
//    int count = 5;
//    const char* msg = "hello world !\n";
//    int len = strlen(msg);
//
//    while(count--)
//    {
//        write(fd, msg, len);
//
//    }
//    close(fd);
//    return 0;
//
//}
//
//
//int main()
//{
//	int fd = open("myfile.txt", O_RDONLY);
//	if(fd < 0)
//	{
//		perror("open");
//		return 1;
//	}
//	
//	const char* msg = "hello world !\n";
//	char buf[1024];
//	while(1)
//	{
//		ssize_t s = read(fd, buf, strlen(msg));//类比write
//		if(s > 0)
//		{
//			printf("%s", buf);
//		}
//		else
//		{
//			break;
//		}
//	}
//	close(fd);
//	return 0;
//}


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int main()
{
	umask(0);
	close(1);
	int fd = open("myfile.txt", O_WRONLY|O_CREAT, 00644);
	if(fd < 0)
	{
		perror("open");
		return 1;
	}
	printf("hello world!\n");
	printf("hello world!\n");
	printf("hello world!\n");
	printf("hello world!\n");
	printf("hello world!\n");
	printf("fd: %d\n", fd);
	
	fflush(stdout);
	close(fd);
	return 0;
}
