#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int scull_init(void) {
    printk(KERN_ALERT "scull module loaded\n");
    return 0;
}

static void scull_exit(void) {
    printk(KERN_ALERT "scull module unloaded\n");
}

module_init(scull_init);
module_exit(scull_exit);
