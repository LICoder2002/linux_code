#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
	umask(0);
  
  close(2);
  close(0);

	int fd1 = open("log1.txt", O_RDONLY | O_CREAT, 0666);
	int fd2 = open("log2.txt", O_RDONLY | O_CREAT, 0666);
	int fd3 = open("log3.txt", O_RDONLY | O_CREAT, 0666);
	int fd4 = open("log4.txt", O_RDONLY | O_CREAT, 0666);
  int fd5 = open("log5.txt", O_RDONLY | O_CREAT, 0666);

  printf("fd1:%d\n", fd1);
	printf("fd2:%d\n", fd2);
	printf("fd3:%d\n", fd3);
	printf("fd4:%d\n", fd4);
	printf("fd5:%d\n", fd5);
	return 0;
}

