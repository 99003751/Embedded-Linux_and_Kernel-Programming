#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#define MAX_SIZE 32
struct cdev cdev;
struct device *pdev; //global
struct class *pclass; //global
dev_t pdevid;
int ndevices=1;
unsigned char *pbuffer;
int rd_offset=0;
int wr_offset=0;
int buflen=0;
int rcount,wcount;
static struct task_struct *task1;
static struct task_struct *task2;
int flag=0;
int flag1=1;
int count=50;
module_param(count,int,S_IRUGO);

wait_queue_head_t w1;

static int val=100;


int pseudo_open(struct inode* inode , struct file* file)
{
printk("Pseudo--open method\n");
return 0;
}
int pseudo_close(struct inode* inode , struct file* file)
{
printk("Pseudo--release method\n");
return 0;
}
ssize_t pseudo_read(struct file * file, char __user * buf , size_t size, loff_t * off)
{

 if(flag==1)
 {
   flag=0;
   int ret;
 printk("Pseudo--read method\n");
 //Read method:-
 if(buflen==0)
//wr_offset-rd_offset==0
 {
  printk("buffer is empty\n");
  return 0;
 }
 rcount = size;
 if(rcount > buflen)
 rcount = buflen;
//min of buflen, size
 ret=copy_to_user(buf, pbuffer + rd_offset,rcount);
 if(ret)
 {
 printk("copy to user failed\n");
 return -EFAULT;
 }
 rd_offset+=rcount;
 buflen -= rcount;
 
 return rcount;
 
 }
}

ssize_t pseudo_write(struct file * file, const char __user * buf , size_t size, loff_t * off)
{

  if(flag1==1)
  {
  flag1=0;
   int ret;
 printk("Pseudo--write method\n");
 if(wr_offset >= MAX_SIZE)
 {
  printk("buffer is full\n");
  return -ENOSPC;
 }
 wcount = size;
 if(wcount > MAX_SIZE - wr_offset)
 wcount = MAX_SIZE - wr_offset;
//min
 ret=copy_from_user(pbuffer + wr_offset,buf,wcount);
if(ret)
 {
 printk("copy from user failed\n");
 return -EFAULT;
 }
 wr_offset+=wcount;
 buflen += wcount;
 return wcount;
  
  
  }
  
}

static int thread_one(void *pargs)
{
    int i;
    wait_event_interruptible(w1, (buflen > 0) );
    flag=1;
    /*for(i=1;i<=count;i++)
    {
        printk("Consumer--%d\n",i);
        val++;
    }*/
	return 0;
}

static int thread_two(void *pargs)
{
    int i;
    /*for(i=1;i<=count;i++)
    {
        printk("Producer--%d\n",i);
        val--;
    }*/
    buflen++;
    flag1=1;
    wake_up_interruptible(&w1);
	return 0;
}

struct file_operations fops = {
.open = pseudo_open,
.release = pseudo_close,
.write= pseudo_write,
.read= pseudo_read
};

static int __init wqdemo_init(void) {        //init_module
  init_waitqueue_head(&w1);
  task1=kthread_run(thread_one, NULL, "thread_A");  //kthread_create + wake_up_process
  task2=kthread_run(thread_two, NULL, "thread_B");
  printk("Thread Demo..welcome\n");
  int i=0;
pclass = class_create(THIS_MODULE, "pseudo_class");
int ret;
ret=alloc_chrdev_region(&pdevid, 0, ndevices, "pseudo_sample");
if(ret) {
printk("Pseudo: Failed to register driver\n");
return -EINVAL;
}
cdev_init(&cdev, &fops);
kobject_set_name(&cdev.kobj,"pdevice%d",i);
ret = cdev_add(&cdev, pdevid, 1);
printk("Successfully registered,major=%d,minor=%d\n",
MAJOR(pdevid), MINOR(pdevid));
printk("Pseudo Driver Sample..welcome\n");
pdev = device_create(pclass, NULL, pdevid, NULL, "psample%d",i);
pbuffer = kmalloc(MAX_SIZE, GFP_KERNEL);
  return 0;
}
static void __exit wqdemo_exit(void) {       //cleanup_module
  /*if(task1)
      kthread_stop(task1);
  if(task2)
      kthread_stop(task2);*/
   cdev_del(&cdev);
class_destroy(pclass);
unregister_chrdev_region(pdevid, ndevices);
printk("Pseudo Driver Sample..Bye\n");
device_destroy(pclass, pdevid);
kfree(pbuffer);
  printk("Thread Demo,Leaving the world,val=%d\n",val);
}

module_init(wqdemo_init);
module_exit(wqdemo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rajesh Sola");
MODULE_DESCRIPTION("Thread Example Module");
