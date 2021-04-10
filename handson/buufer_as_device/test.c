#include<stdio.h>
#include <stdlib.h>
#include<fcntl.h>
int main()
{
 printf("hel;lohdjfh");
int fd=open("/dev/psample0",O_RDWR);
printf("\n%d\n",fd);
if(fd<0)
	perror("open");
char str[]="abcdxyz";
int nbytes=write(fd,str,36);
printf("\n%d\n",nbytes);
if(nbytes<0) {
perror("write");
}
char buf[64];
int maxlen=10;
nbytes=read(fd,buf,maxlen);
printf("\n%d\n",nbytes);

nbytes=read(fd,buf,maxlen);
printf("\n%d\n",nbytes);

nbytes=read(fd,buf,maxlen);
printf("\n%d\n",nbytes);

nbytes=read(fd,buf,maxlen);
printf("\n%d\n",nbytes);


if(nbytes<0) {
perror("read");
}
close(fd);
}
