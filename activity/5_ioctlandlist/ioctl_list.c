#include <linux/fs.h>

#include <linux/init.h>

#include <linux/module.h>

#include <linux/kernel.h>

#include <linux/cdev.h>

#include <linux/slab.h>

#include <linux/device.h>

#include <linux/uaccess.h>

#include <linux/kfifo.h>
#include<linux/ioctl.h>
 

#define MAX_SIZE 32
#define IOC_MAGIC 'p'
#define MY_IOCTL_RESET  _IO(IOC_MAGIC, 3)
#define MY_IOCTL_AVAIL  _IO(IOC_MAGIC, 2)
#define MY_IOCTL_LEN	_IO(IOC_MAGIC, 1)
#define MY_MACIG ']'
#define READ_IOCTL _IOR(MY_MACIG, 0, int)
#define WRITE_IOCTL _IOW(MY_MACIG, 1, int)

void printdata(void);
struct device *pdev; //global

struct class *pclass; //global

struct cdev cdev;
LIST_HEAD(mylist);
struct sample {
  int x;
  int y;
  int z;
  struct list_head entry;
};


int ndevices=1;

int i=0;

unsigned char *pbuffer;

int rd_offset=0;

int wr_offset=0;

int buflen=0;

char tbuf;
int n;
char ret;

int rcount;

int wcount;

struct kfifo myfifo;

static long pseudo_ioctl(struct file *file,unsigned int cmd, unsigned long arg)
{
char buf[50];
int len=20;
printk("Pseudo--ioctl method\n");
switch (cmd) {
case WRITE_IOCTL :
printk("ioctl--listnode create is %d\n",arg);
int i;
struct sample *ps;
for(i=1;i<=arg;i++) {
ps=kmalloc(sizeof(struct sample), GFP_KERNEL);
ps->x=i*100;
ps->y=i*100+10;
list_add_tail(&ps->entry, &mylist);
}    printk("Hello World..welcome\n");
break;
case READ_IOCTL:
printk("ioctl--listnode print");
printdata();
break;
}
return 0;
}


void printdata(void) {
  struct list_head* pcur;
  struct sample *ps;
  list_for_each(pcur,&mylist) {
    ps=list_entry(pcur, struct sample,entry); //container_of
    printk("Successfully LISTED,X value=%d,Y Value=%d\n", ps->x, ps->y);
    //print ps->x,ps->y
  }
}

struct file_operations fops =
{

  .unlocked_ioctl = pseudo_ioctl

};

dev_t pdevid;

static int __init psuedo_init(void)

{

  int ret, i=0;

  pclass = class_create(THIS_MODULE, "pseudo_class");

  ret=alloc_chrdev_region(&pdevid, 0, ndevices, "pseudo_file_driver_sample");

  cdev_init(&cdev, &fops);

  pbuffer = kmalloc(MAX_SIZE, GFP_KERNEL);

  kfifo_init(&myfifo, pbuffer,8);

  kobject_set_name(&cdev.kobj,"pdevice%d",i);

  ret = cdev_add(&cdev, pdevid, 1);

  pdev = device_create(pclass, NULL, pdevid, NULL, "psample%d",i);

  if(ret) {

  printk("Pseudo: Failed to register driver\n");

  return -EINVAL;

  }

  printk("Successfully registered,major=%d,minor=%d\n",

  MAJOR(pdevid), MINOR(pdevid));

  printk("Pseudo Driver Sample..welcome\n");

  return 0;

}

static void __exit psuedo_exit(void) 

{

  device_destroy(pclass, pdevid);

  cdev_del(&cdev);

  unregister_chrdev_region(pdevid, ndevices);

  kfifo_free(&myfifo);

  class_destroy(pclass);

  kfree(pbuffer);
  
    struct list_head *pcur,*pbak;
  struct sample *ps;
  list_for_each_safe(pcur,pbak, &mylist) {
    ps=list_entry(pcur,struct sample, entry); //container_of
    kfree(ps);
  }
  printk("Bye,Leaving the world\n");

  printk("Pseudo Driver Sample..Bye\n");

}

module_init(psuedo_init);

module_exit(psuedo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Shrinidhi V katti");

MODULE_DESCRIPTION("A pseudo sample Module");
