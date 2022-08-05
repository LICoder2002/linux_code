#include "progressbar.h"

void progress()
{
  char bar[NUM];
  memset(bar, '\0',sizeof(bar));

  int cnt = 0;
  while(cnt <= 100)
  {
    printf("加载中：【%-100s】【%d%%】\r", bar, cnt);
    fflush(stdout);
    bar[cnt++] = STYLE;

    usleep(200000);

  }
  printf("\n");
}
