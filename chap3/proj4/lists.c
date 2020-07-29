#include <linux/init.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

static struct list_head color_list;

struct color {
    int red;
    int green;
    int blue;
    struct list_head list;
};

static void output_list(void) {
    struct color * pos;
    printk(KERN_INFO "List elements:\n");
    list_for_each_entry(pos, &color_list, list) {
        printk(KERN_INFO "%d %d %d", pos->red, pos->green, pos->blue);
    }
    printk(KERN_INFO "End of list\n");
}

/* This function is called when the module is loaded. */
static int task_init(void) {
    struct color * red_color;
    struct color * green_color;
    struct color * blue_color;
    struct color * black_color;

    INIT_LIST_HEAD(&color_list);

    red_color = kmalloc(sizeof(*red_color), GFP_KERNEL);
    green_color = kmalloc(sizeof(*green_color), GFP_KERNEL);
    blue_color = kmalloc(sizeof(*blue_color), GFP_KERNEL);
    black_color = kmalloc(sizeof(*black_color), GFP_KERNEL);

    red_color->red = 255;
    red_color->green = 0;
    red_color->blue = 0;

    green_color->red = 0;
    green_color->green = 255;
    green_color->blue = 0;

    blue_color->red = 0;
    blue_color->green = 0;
    blue_color->blue = 255;

    black_color->red = 255;
    black_color->green = 255;
    black_color->blue = 255;

    list_add_tail(&red_color->list, &color_list);
    list_add_tail(&green_color->list, &color_list);
    list_add_tail(&blue_color->list, &color_list);
    list_add_tail(&black_color->list, &color_list);

    output_list();

    return 0;
}

/* This function is called when the module is removed. */
static void task_exit(void) {
    struct color * pos;
    struct color * next;
    printk(KERN_INFO "Destroying the list\n");
    list_for_each_entry_safe(pos, next, &color_list, list) {
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
