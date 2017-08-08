#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include "scull.h"
MODULE_LICENSE("Dual BSD/GPL");

struct scull_dev *scull_devices;

int scull_minor = 0;
int scull_major = SCULL_MAJOR;
int scull_nr_devs = SCULL_NR_DEVS;
int scull_quantum = SCULL_QUANTUM;
int scull_qset = SCULL_QSET;
module_param(scull_minor, int , S_IRUGO);
module_param(scull_major, int, S_IRUGO); 
module_param(scull_nr_devs, int, S_IRUGO); 
module_param(scull_quantum, int, S_IRUGO); 
module_param(scull_qset, int, S_IRUGO); 

static int scull_trim(struct scull_dev *dev)
{
    return 0;
}

static loff_t scull_llseek(struct file *filep, loff_t off, int whence)
{
    return 0;
}

static ssize_t scull_read(struct file *filep, char __user *buf, size_t count, loff_t *pos)
{
   return 0;
}

static ssize_t scull_write(struct file *filep, const char __user *buf, size_t count, loff_t *pos)
{
    return 0;
}

static long scull_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
   return 0;
} 

static int scull_open(struct inode *inode, struct file *filep)
{
    struct scull_dev *dev;
    
    dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    /* store the pointer to scull_dev for other methods */
    filep->private_data = dev;
    
    /* Now trim to 0 the length of the device if it is open as write-only */ 
    if ((filep->f_flags & O_ACCMODE) == O_WRONLY) {
        /* ignore errors */
        scull_trim(dev);
    }

    /* success */
    return 0;
}

static int scull_release(struct inode *inode, struct file *filep)
{
    return 0;
}


struct file_operations scull_fops = {
    .owner          = THIS_MODULE,
    .llseek         = scull_llseek,
    .read           = scull_read,
    .write          = scull_write,
    .unlocked_ioctl = scull_ioctl,
    .open           = scull_open,
    .release        = scull_release,   
};

static void scull_setup_cdev(struct scull_dev *dev, int index)
{
    int err, devno = MKDEV(scull_major, scull_minor + index);

    cdev_init(&dev->cdev, &scull_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops   = &scull_fops;

    err = cdev_add(&dev->cdev, devno, 1);
    /* Fail garcefully if needed */
    if (err) {
        printk(KERN_NOTICE "Error %d while adding scull%d\n", err, index);    
    }
}

static int scull_init(void) {
    int result, i;
    dev_t dev = 0;

    if (scull_major) {
        dev = MKDEV(scull_major, scull_minor);
        result = register_chrdev_region(dev, scull_nr_devs, "scull");
    } else {
        result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
        scull_major = MAJOR(dev);
    }

    if (result < 0) {
        printk(KERN_WARNING "scull: can't get major dev number\n");
    }

    scull_devices = kmalloc((scull_nr_devs * sizeof(struct scull_dev)), GFP_KERNEL);
    if (!scull_devices) {  
        result = -ENOMEM;
        goto fail;
    }

    memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));

    /* Initialize each device */
    for (i = 0; i < scull_nr_devs; i++) {
        scull_devices[i].quantum = scull_quantum;
        scull_devices[i].qset = scull_qset;
        scull_setup_cdev(&scull_devices[i], i);    
    }
    printk(KERN_ALERT "scull: module loaded\n");
    return 0;

fail:
    printk("Failure handle unimplemented");
    return result;
}

static void scull_exit(void) {
    dev_t dev = MKDEV(scull_major, scull_minor);

    unregister_chrdev_region(dev, scull_nr_devs);
    printk(KERN_ALERT "scull: module unloaded\n");
}

module_init(scull_init);
module_exit(scull_exit);
