#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include<linux/semaphore.h>
#include<linux/list.h>
#include<linux/slab.h>
struct sample {
  int x;
  int y;
  struct list_head entry;
};
LIST_HEAD(mylist);
int N = 5;
int count = 0;
void printdata(void) {
  struct list_head* pr_current;
  struct sample *ps;
  list_for_each(pr_current,&mylist) {
    ps=list_entry(pr_current, struct sample,entry);
    printk("%d\n %d\n",ps->x,ps->y);
  }
}

static struct task_struct *task1;
static struct task_struct *task2;
static struct task_struct *task3;

int val=100;

const int max=10;
static DEFINE_SEMAPHORE(s1);
static DEFINE_SEMAPHORE(s2);
static DEFINE_SEMAPHORE(s3);

static int thread_one(void *pargs)
{
	int i;
	printk("Welcome Thread A Consumer");
	down_interruptible(&s3);
	down_interruptible(&s2);
	printdata();
	up(&s3);
	up(&s2);
	do_exit(0);
	return 0;

}


static int thread_two(void *pargs)
{
	struct sample *ps;
	int i;
	printk("Welcome Thread B Producer 1 ");
	down_interruptible(&s1);
	for(i=1;i<=N;i++)
  {
    ps=kmalloc(sizeof(struct sample), GFP_KERNEL);
    ps->x=i*100;
    ps->y=i*100+10;
    list_add_tail(&ps->entry, &mylist);
		if(kthread_should_stop())
			break;
	 }
	up(&s1);
	up(&s2);
	do_exit(0);
	return 0;

}

static int thread_three(void *pargs)
{
	struct sample *ps;
	int i;
	printk("Welcome Thread C Producer 2 ");
	down_interruptible(&s2);
	down_interruptible(&s1);
	for(i=1;i<=N;i++)
  {
    ps=kmalloc(sizeof(struct sample), GFP_KERNEL);
    ps->x=i*100;
    ps->y=i*100+10;
    list_add_tail(&ps->entry, &mylist);
		if(kthread_should_stop())
			break;
	 }
	up(&s1);
	up(&s2);
	up(&s3);
	do_exit(0);
	return 0;

}

static int __init thread_init(void)
{
	sema_init(&s1,1);
	sema_init(&s2,0);
	sema_init(&s3,0);
	task1=kthread_run(thread_one, NULL, "thread_A");
	//kthread_create + wake_up_process
	task2=kthread_run(thread_two, NULL, "thread_B");
	task3=kthread_run(thread_three, NULL, "thread_C");
	return 0;

}

static void __exit thread_exit(void)
{
  struct list_head *pr_current,*pbak;
  struct sample *ps;
  printk("Leaving, Bye");
  list_for_each_safe(pr_current,pbak, &mylist)
  {
    ps=list_entry(pr_current,struct sample, entry);
    kfree(ps);
  }
	printdata();
	printk("Empty data");
}

module_init(thread_init);
module_exit(thread_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kiran");
MODULE_DESCRIPTION("Thread Module List");
