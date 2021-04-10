#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>

static DEFINE_SEMAPHORE(s1);
static DEFINE_SEMAPHORE(s2); 


static struct task_struct *task1;
static struct task_struct *task2;

int count=50;
module_param(count,int,S_IRUGO);

wait_queue_head_t w1;

static int val=100;
//static int buflen=0;

static int thread_one(void *pargs)
{
    int i;
   down_interruptible(&s2);
   down_interruptible(&s1);

    for(i=1;i<=count;i++)
    {
        printk("Consumer--%d\n",i);
        val++;
    }
    up(&s1);
	return 0;
}

static int thread_two(void *pargs)
{
    down_interruptible(&s1);
    int i;
    for(i=1;i<=count;i++)
    {
        printk("Producer--%d\n",i);
        val--;
    }
    up(&s1);
    up(&s2);
	return 0;
}

static int __init wqdemo_init(void) {        //init_module
  //init_waitqueue_head(&w1);
  sema_init(&s1,1);	//p
  sema_init(&s2,0);	//c
  task1=kthread_run(thread_one, NULL, "thread_A");  //kthread_create + wake_up_process
  task2=kthread_run(thread_two, NULL, "thread_B");
  printk("Thread Demo..welcome\n");
  return 0;
}
static void __exit wqdemo_exit(void) {       //cleanup_module
  /*if(task1)
      kthread_stop(task1);
  if(task2)
      kthread_stop(task2);*/
  printk("Thread Demo,Leaving the world,val=%d\n",val);
}

module_init(wqdemo_init);
module_exit(wqdemo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shrinidhi V Katti");
MODULE_DESCRIPTION("Thread Example Module");
