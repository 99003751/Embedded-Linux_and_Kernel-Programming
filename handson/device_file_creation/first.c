#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
struct device *pdev; //global
struct class *pclass; //global
dev_t pdevid;
int ndevices=1;
static int __init psuedo_init(void)
{
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
return 0;

}
static void __exit psuedo_exit(void) {
cdev_del(&cdev);
class_destroy(pclass);
unregister_chrdev_region(pdevid, ndevices);
printk("Pseudo Driver Sample..Bye\n");
device_destroy(pclass, pdevid);
}
module_init(psuedo_init);
module_exit(psuedo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your name");
MODULE_DESCRIPTION("A pseudo sample Module");
