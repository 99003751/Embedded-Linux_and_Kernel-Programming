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
struct device *pdev; //global

struct class *pclass; //global

struct cdev cdev;

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
case MY_IOCTL_LEN :
printk("ioctl--kfifo length is %d\n",
kfifo_len(&myfifo));
break;
case MY_IOCTL_AVAIL:
printk("ioctl--kfifo avail is %d\n",
kfifo_avail(&myfifo));
break;
case MY_IOCTL_RESET:
printk("ioctl--kfifo got reset\n");
kfifo_reset(&myfifo);
break;
case READ_IOCTL:	
		n=copy_to_user((char __user*)arg,buf, 20);
		 if(n)
   		{
     		 printk("copy");
      		return -EFAULT;
   		}
		printk("read method %s",buf);
		printk("read method %d",arg);
		break;
	
case WRITE_IOCTL:
		n=copy_from_user(buf, (char __user*)arg, len);
		 if(n)
   		{
      		printk("copy");
      		return -EFAULT;
   		}
		printk("write method %d---------%d",arg,n);
		printk("write method %c",buf);
		break;
}
return 0;
}

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

ssize_t pseudo_read(struct file * file, char __user * ubuf , size_t size, loff_t * off)

{

 

//=========================================

if(kfifo_is_empty(&myfifo)) {

printk("buffer is empty\n");

return 0;

}

 rcount = size;

if(rcount > kfifo_len(&myfifo))

rcount = kfifo_len(&myfifo);

tbuf = kmalloc(rcount, GFP_KERNEL);

kfifo_out(&myfifo, tbuf, rcount);

ret=copy_to_user(ubuf, tbuf, rcount);

//error handling

kfree(tbuf);

//==========================================

	  if(buflen==0)

	  {

		  printk("buffer is empty\n");

		  return 0;

	  }

	  rcount = size;

	  if(rcount > buflen)

	   	rcount = buflen;

	    ret=copy_to_user(ubuf, pbuffer + rd_offset,rcount);

	   	  if(ret)

	   	  {

			 printk("copy to user failed\n");

		    return -EFAULT;

	  }

	  rd_offset+=rcount;

	  printk("rd_offset=%d",rd_offset);

	  printk("\nrcount=%d", rcount);

	  printk("\nbufflen=%d", buflen);

	  buflen -= rcount;

	  printk("\nbufflen=%d", buflen);

	  printk("Pseudo--read method\n");

	  return rcount;

}

ssize_t pseudo_write(struct file * file, const char __user * ubuf , size_t size, loff_t * off)

{

//=========================

  if(kfifo_is_full(&myfifo))

  {

    printk("buffer is full\n");

    return -ENOSPC;

  }

   wcount = size;

if(wcount > kfifo_avail(&myfifo))

wcount = kfifo_avail(&myfifo);

char *tbuf=kmalloc(wcount, GFP_KERNEL);

ret=copy_from_user(tbuf, ubuf, wcount);

//error handling if copy_form_user

kfifo_in(&myfifo, tbuf, wcount);

kfree(tbuf);

  if(wr_offset >= MAX_SIZE)

 {

    printk("buffer is full\n");

    return -ENOSPC;

  }

int wcount = size;

  if(wcount > MAX_SIZE - wr_offset)

     wcount = MAX_SIZE - wr_offset;

int ret=copy_from_user(pbuffer + wr_offset,ubuf,wcount);

if(ret)

 {

   printk("copy from user failed\n");

   return -EFAULT;

}

wr_offset+=wcount;

printk("\nwr_offset=%d", wr_offset);

printk("\nwcount=%d", wcount);

printk("\nbufflen=%d", buflen);

buflen += wcount;

printk("\nbufflen=%d", buflen);

printk("Pseudo--write method\n");

return wcount;

}

struct file_operations fops =

{

  .open = pseudo_open,

  .release = pseudo_close,

  .write = pseudo_write,

  .read = pseudo_read,
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

  printk("Pseudo Driver Sample..Bye\n");

}

module_init(psuedo_init);

module_exit(psuedo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("Rajat");

MODULE_DESCRIPTION("A pseudo sample Module");
