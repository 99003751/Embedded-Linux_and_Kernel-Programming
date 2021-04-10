#include<stdio.h>
#include <stdlib.h>
#include<fcntl.h>
#include<linux/ioctl.h>
#define IOC_MAGIC 'p'
#define MY_IOCTL_RESET  _IO(IOC_MAGIC, 3)
#define MY_IOCTL_AVAIL  _IO(IOC_MAGIC, 2)
#define MY_IOCTL_LEN	_IO(IOC_MAGIC, 1)
#define MY_MACIG ']'
#define READ_IOCTL _IOR(MY_MACIG, 0, int)
#define WRITE_IOCTL _IOW(MY_MACIG, 1, int)
void printdata(void);
int main()
{
int nbytes,ret;
 printf("hel;lohdjfh");
int fd=open("/dev/psample0",O_RDWR);
printf("\n%d\n",fd);
if(fd<0)
	perror("open");
char str[]="abcdxyz";
/*int nbytes=write(fd,str,10);
printf("\n%d\n",nbytes);

nbytes=write(fd,str,10);
printf("\n%d\n",nbytes);

nbytes=write(fd,str,10);
printf("\n%d\n",nbytes);

nbytes=write(fd,str,10);
printf("\n%d\n",nbytes);
if(nbytes<0){
perror("write");
}
char buf[64];
int maxlen=10,ret;
nbytes=read(fd,buf,maxlen);
printf("\n%d\n",nbytes);

nbytes=read(fd,buf,maxlen);
printf("\n%d\n",nbytes);

nbytes=read(fd,buf,maxlen);
printf("\n%d\n",nbytes);

nbytes=read(fd,buf,maxlen);
printf("\n%d\n",nbytes);*/

ret=ioctl(fd, MY_IOCTL_LEN);
if(ret<0)
{
perror("ioctl");
exit(3);
}
ret=ioctl(fd, MY_IOCTL_AVAIL);
if(ret<0)
{
perror("ioctl");
exit(3);
}

ret=ioctl(fd, MY_IOCTL_AVAIL);
if(ret<0)
{
perror("ioctl");
exit(3);
}

ret=ioctl(fd, MY_IOCTL_RESET);
if(ret<0) {
perror("ioctl");
exit(3);
}

ret=ioctl(fd,WRITE_IOCTL,123);
if(ret<0)
{
perror("ioctl");
exit(3);
}

unsigned long abc;
ret=ioctl(fd,READ_IOCTL);
if(ret<0)
{
perror("ioctl");
exit(3);
}

if(nbytes<0) {
perror("read");
}
close(fd);
}
