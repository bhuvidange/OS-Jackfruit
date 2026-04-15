#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "monitor_ioctl.h"

#define DEVICE_NAME "container_monitor"

static int major;

// Simple struct
struct container_node {
    pid_t pid;
    size_t soft_limit;
    size_t hard_limit;
    struct list_head list;
};

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    struct monitor_config config;

    switch (cmd) {
        case REGISTER_CONTAINER:
            copy_from_user(&config, (void *)arg, sizeof(config));
            printk(KERN_INFO "Register PID %d\n", config.pid);
            break;

        case SET_LIMITS:
            copy_from_user(&config, (void *)arg, sizeof(config));
            printk(KERN_INFO "Set limits for PID %d\n", config.pid);
            break;

        default:
            return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .unlocked_ioctl = device_ioctl,
};

static int __init monitor_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_INFO "Monitor loaded\n");
    return 0;
}

static void __exit monitor_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Monitor unloaded\n");
}

module_init(monitor_init);
module_exit(monitor_exit);

MODULE_LICENSE("GPL");