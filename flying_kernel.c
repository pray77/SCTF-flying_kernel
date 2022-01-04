#include <linux/module.h>
#include <linuxersion.h>
#include <linux/kernel.h>
#include <linuxpes.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linuxev.h>
#include <asm/uaccess.h>
#include <linuxab.h>
 
static char *sctf_buf = NULL;
static struct class *devClass;
static struct cdev cdev;
static dev_t seven_dev_no;
 
static ssize_t seven_write(struct file *filp, const char __user *buf, u_int64_t len, loff_t *f_pos);
 
static long seven_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
 
static int seven_open(struct inode *i, struct file *f);
 
static int seven_close(struct inode *i, struct file *f);
 
 
 
static struct file_operations seven_fops =
        {
                .owner = THIS_MODULE,
                .open = seven_open,
                .release = seven_close,
                .write = seven_write,
                .unlocked_ioctl = seven_ioctl
        };
 

static int __init seven_init(void)
{
    if (alloc_chrdev_region(&seven_dev_no, 0, 1, "seven") < 0)
    {
        return -1;
    }
    if ((devClass = class_create(THIS_MODULE, "chardrv")) == NULL)
    {
        unregister_chrdev_region(seven_dev_no, 1);
        return -1;
    }
    if (device_create(devClass, NULL, seven_dev_no, NULL, "seven") == NULL)
    {
        class_destroy(devClass);
        unregister_chrdev_region(seven_dev_no, 1);
        return -1;
    }
    cdev_init(&cdev, &seven_fops);
    if (cdev_add(&cdev, seven_dev_no, 1) == -1)
    {
        device_destroy(devClass, seven_dev_no);
        class_destroy(devClass);
        unregister_chrdev_region(seven_dev_no, 1);
        return -1;
    }
    return 0;
}
 

static void __exit seven_exit(void)
{
    unregister_chrdev_region(seven_dev_no, 1);
    cdev_del(&cdev);
}
 

ssize_t seven_write(struct file *filp, const char __user *buf, u_int64_t len, loff_t *f_pos)
{
    if (sctf_buf)
    {
        if (len <= 0x80)
        {
            printk(KERN_INFO "write()\n" );
            u_int64_t offset = 0x80 - len;
            copy_from_user((u_int64_t)((char *)sctf_buf) + offset, buf, len);
        }
    }
    else
    {
        printk("What are you doing?");
    }

    return len;
}
 
 
// ioctl函数命令控制
long seven_ioctl(struct file *filp, unsigned int cmd, unsigned long size)
{
    int retval = 0;
    switch (cmd) {

        case 0x5555://add
            if (size == 0x80)
            {
                sctf_buf = (char *)kmalloc(size,GFP_KERNEL);
                printk("Add Success!\n");
            }
            else
            {
                printk("It's not that simple\n");
            }
            break;

        case 0x6666:
            if (sctf_buf)
            {
                kfree(sctf_buf);
            }
            else
            {
                printk("What are you doing?");
                retval = -1;
            }
            break;

        case 0x7777:
            if (sctf_buf)
            {
                printk(sctf_buf);
            }
            break;

        default:
            retval = -1;
            break;
    }   
 
    return retval;
}
 
 
static int seven_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO "open()\n");
    return 0;
}
 
static int seven_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO "close()\n");
    return 0;
}
 
module_init(seven_init);
module_exit(seven_exit);
 
MODULE_LICENSE("GPL");