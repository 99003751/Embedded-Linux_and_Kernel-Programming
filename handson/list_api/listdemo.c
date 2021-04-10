#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
void printdata(void);

struct sample {
  int x;
  int y;
  int z;
  struct list_head entry;
};

LIST_HEAD(mylist);

int __init listdemo_init(void) {        //init_module
  int i;
  struct sample *ps;
  for(i=1;i<=10;i++) {
    ps=kmalloc(sizeof(struct sample), GFP_KERNEL);
    ps->x=i*100;
    ps->y=i*100+10;
    list_add_tail(&ps->entry, &mylist);
  }    
  printdata();
  printk("Hello World..welcome\n");
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

void __exit listdemo_exit(void) {       //cleanup_module
  struct list_head *pcur,*pbak;
  struct sample *ps;
  list_for_each_safe(pcur,pbak, &mylist) {
    ps=list_entry(pcur,struct sample, entry); //container_of
    kfree(ps);
  }
  printk("Bye,Leaving the world\n");
}

module_init(listdemo_init);
module_exit(listdemo_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shrinidhi V katti");
MODULE_DESCRIPTION("A Hello, World Module");
