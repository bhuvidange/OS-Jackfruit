#ifndef MONITOR_IOCTL_H
#define MONITOR_IOCTL_H

#include <linux/ioctl.h>
#include <sys/types.h>

#define DEVICE_NAME "/dev/container_monitor"

// IOCTL commands
#define REGISTER_CONTAINER _IOW('a', 1, int)
#define UNREGISTER_CONTAINER _IOW('a', 2, int)
#define SET_LIMITS _IOW('a', 3, struct monitor_config)

// Config struct
struct monitor_config {
    pid_t pid;
    size_t soft_limit; // in bytes
    size_t hard_limit; // in bytes
};

#endif