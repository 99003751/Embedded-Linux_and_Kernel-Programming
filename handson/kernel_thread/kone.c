#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include<linux/kthread.h>
#include<linux/delay.h>

static struct task_struct *task1;
static struct task_struct *task2;

int val=100;
const int max=1000000;
static int thread_one(void *pargs){

while(!kthread_should_stop())
{
int i;
for(i=1;i<=max;i++)
{
 val++;
 if(kthread_should_stop())
 break;
 msleep(2000);
}
printk("Thread A--%d\n",val);
//do_exit(0);
return 0;
}
}
static int thread_two(void *pargs){
while(!kthread_should_stop())
{
int i;
for(i=1;i<=max;i++)
{
val--;
if(kthread_should_stop())
break;
msleep(2000);
}
printk("Thread B--%d\n",val);
//do_exit(0);
return 0;
}
}
static int __init psuedo_init(void)
{
 task1=kthread_run(thread_one, NULL, "thread_A");
 task2=kthread_run(thread_two, NULL, "thread_B");
 return 0;
}


static void __exit psuedo_exit(void)
{
 /*
 if(task1)
 {

  kthread_stop(task1);
 }
if(task2)
{
 kthread_stop(task2);
}*/
printk("Thread Bexit------------%d\n",val);
}
module_init(psuedo_init);
module_exit(psuedo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("A pseudo sample Module");
