#include<stdio.h>
#include<unistd.h>
#include<sys/fcntl.h>
int main()
{
  int fd = open("test.1",O_WRONLY|O_CREAT,0664);
  if(fd<0)
  {
    printf("open!\n");
    return 0;
  }
  dup2(fd,1);
  printf("heeee\n");
  return 0;
}
