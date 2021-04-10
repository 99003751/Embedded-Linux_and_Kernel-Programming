#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
SYSCALL_DEFINE2(mytestthree, const char __user *,ch,char __user *,gett)
{
   char buf[50],rev[50];
   int n,i,j;
   int len=0;
   n=copy_from_user(buf,ch,50);
   if(n)
   {
      printk("copy");
      return -EFAULT;
   }
   while(buf[len] != '\0')
   	len++;
   j=len-1;
   
   for(i=0;i<len;i++)
   {
     rev[i] = buf[j];
    j--;
   }
   rev[len]='\0';
   n=copy_to_user(gett,rev,50);
   if(n)
   {
    return -EFAULT;
   }
   printk("%s",rev);
   return 0;
}
