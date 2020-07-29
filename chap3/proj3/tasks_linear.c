#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

/* This function is called when the module is loaded. */
static int task_init(void) {

    struct task_struct * task;

    for_each_process(task) {
        printk(KERN_INFO "command = [%s] pid = [%d] state = [%ld]\n", task->comm, task->pid, task->state);
    }

    return 0;
}

/* This function is called when the module is removed. */
static void task_exit(void) {}

/* Macros for registering module entry and exit points. */
module_init( task_init );
module_exit( task_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");
