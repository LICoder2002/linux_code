#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//int main()
//{
//	close(1);
//	int fd = open("log.txt", O_WRONLY|O_APPEND|O_CREAT, 0666);
//	if(fd < 0)
//	{
//		perror("open");
//		return 1;
//	}
//	
//	printf("hello World\n");
//	printf("hello World\n");
//	printf("hello World\n");
//	printf("hello World\n");
//	printf("hello World\n");
//	
//	fflush(stdout);
//	close(fd);
//	return 0;
//}

//int main()
//{
//    close(0);
//    int fd = open("log.txt", O_RDONLY | O_CREAT, 0666);
//    if(fd < 0)
//    {
//        perror("open");
//        return 1;
//    }
//
//    char str[1024];
//    while(scanf("%s", str) != EOF)
//    {
//        printf("%s\n", str);
//    }
//
//    close(fd);
//    return 0;
//}
//

//int main() 
//{
//    printf("hello printf\n"); //stdout
//    perror("perror"); // stderr
//
//    fprintf(stdout, "stdout: hello fprintf\n"); //stdout
//    
//    fprintf(stderr, "stderr: hello fprintf\n"); //stdout
//
//    return 0;
//}
//
//
//#include <stdio.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <fcntl.h>
//int main()
//{
//	int fd = open("log.txt", O_WRONLY | O_CREAT, 0666);
//	if (fd < 0){
//		perror("open");
//		return 1;
//	}
//	close(1);
//	dup2(fd, 1);
//	printf("hello printf\n");
//	fprintf(stdout, "hello fprintf\n");
//	return 0;
//}
//
#include <stdio.h>
#include <string.h>
int main()
{
	const char *msg0="hello printf\n";
	const char *msg1="hello fwrite\n";
	const char *msg2="hello write\n";
	
	printf("%s", msg0);
	fwrite(msg1, strlen(msg0), 1, stdout);
	write(1, msg2, strlen(msg2));

	fork();
	return 0;
}
