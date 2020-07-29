#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

static int start = 25;
module_param(start, int, 0);

static struct list_head collatz_list;

struct collatz_node {
    int val;
    struct list_head list;
};

static void output_list(void) {
    struct collatz_node * pos;
    printk(KERN_INFO "List elements:\n");
    list_for_each_entry(pos, &collatz_list, list) {
        printk(KERN_INFO "val: %d", pos->val);
    }
    printk(KERN_INFO "End of list\n");
}

/* This function is called when the module is loaded. */
static int task_init(void) {
    struct collatz_node * node;
    int cur;

    INIT_LIST_HEAD(&collatz_list);

    if (start < 1) {
        printk(KERN_INFO "Start value should be > 1");
        return 0;
    }

    cur = start;
    while (cur != 1) {
        node = kmalloc(sizeof(*node), GFP_KERNEL);
        node->val = cur;
        list_add_tail(&node->list, &collatz_list);
        if (cur % 2 == 0) {
            cur = cur / 2;
        } else {
            cur = 3 * cur + 1;
        }
    }
    node = kmalloc(sizeof(*node), GFP_KERNEL);
    node->val = cur;
    list_add_tail(&node->list, &collatz_list);

    output_list();

    return 0;
}

/* This function is called when the module is removed. */
static void task_exit(void) {
    struct collatz_node * pos;
    struct collatz_node * next;
    printk(KERN_INFO "Destroying the list\n");
    list_for_each_entry_safe(pos, next, &collatz_list, list) {
        list_del(&pos->list);
        kfree(pos);
    }
    output_list();
}

/* Macros for registering module entry and exit points. */
module_init( task_init );
module_exit( task_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module");
MODULE_AUTHOR("SGG");
