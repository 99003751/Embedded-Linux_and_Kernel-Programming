#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
SYSCALL_DEFINE0(mytestfour)
{
	int n;
   struct task_struct *task = current; // getting global current pointer
    printk(KERN_NOTICE "assignment: current process: %s, PID: %d", task->comm, task->pid);
  
   do
    {
    task = task->parent;
    printk(KERN_NOTICE "assignment: parent process: %s, PID: %d", task->comm, task->pid);
    }while(task->pid!=0);
    struct task_struct *task1,*taskp;
    for_each_process(task1) {/* this pointlessly prints the name and PID of each task */
    taskp=task1->parent;
    printk("%s[%d]-----p[%d]\n", task1->comm, task1->pid,taskp->pid);
    }
 
   return 0;
}
