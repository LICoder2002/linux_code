#include<stdio.h>

int main()
{
    FILE* fp = fopen("bite.txt", "r");
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
