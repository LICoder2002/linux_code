#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        return 1;
    }


   // FILE* fp = fopen("bite.txt", "r");
    FILE* fp = fopen(argv[1], "r");
  //  FILE* fp = fopen("bite.txt", "w");
    if(fp == NULL)
    {
        perror("fopen");
        return 0;
    }

    char buffer[64];
    while(fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        printf("%s", buffer);
    }
  
    
   // fprintf(fp, "linux is so easy!!!");

    fclose(fp);
    return 0;
}
