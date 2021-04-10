#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define __NR_testcall 401
int main() {
printf("hello world");
int ret;
char abc[100];
ret=syscall(__NR_testcall,"shrinidhikatti",abc);
printf("\nreversed string is %s\n", abc);
if(ret<0)
perror("testcall");
return 0;
}
