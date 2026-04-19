# Multi-Container Runtime

## Team Information
- Arushi Punhani – PES2UG24CS086  
- Bhuvi Dange – PES2UG24CS118  

---

## Project Summary

This project implements a lightweight multi-container runtime in C on Linux. It consists of a user-space supervisor that manages multiple containers and a kernel-space module that monitors memory usage. The system supports container lifecycle management, logging, memory enforcement, and concurrent execution.

---

## Build, Load, and Run Instructions

### Build
make

### Load Kernel Module
sudo insmod monitor.ko

### Verify Device
ls -l /dev/container_monitor

### Start Supervisor
sudo ./engine supervisor ./rootfs-base

### Create Root Filesystems
cp -a ./rootfs-base ./rootfs-alpha  
cp -a ./rootfs-base ./rootfs-beta  

### Start Containers
sudo ./engine start alpha ./rootfs-alpha /bin/sh --soft-mib 48 --hard-mib 80  
sudo ./engine start beta ./rootfs-beta /bin/sh --soft-mib 64 --hard-mib 96  

### List Containers
sudo ./engine ps

### View Logs
sudo ./engine logs alpha

### Stop Containers
sudo ./engine stop alpha  
sudo ./engine stop beta  

### Kernel Logs
dmesg | tail

### Unload Module
sudo rmmod monitor

---

## Demo (Screenshots)

1. Multiple containers running under supervisor  
2. `ps` output showing container metadata  
3. Log output captured from containers  
4. CLI commands interacting with supervisor  
5. Soft memory limit warning in logs  
6. Hard memory limit kill in logs  
7. Concurrent container execution  
8. Clean shutdown with no zombie processes  

---

## Engineering Analysis

### 1. Isolation Mechanisms
From the outputs, containers like alpha run with their own PID and root filesystem (rootfs-alpha). This indicates that PID and filesystem isolation are working. Inside the container, `/proc` is mounted, which allows process-related commands to function correctly.

Each container uses a separate rootfs copy, so file changes in one container do not affect others. However, all containers share the same host kernel, which is why kernel logs (`dmesg`) show activity for all containers together.

---

### 2. Supervisor and Process Lifecycle
The supervisor process remains active while containers are started using commands like `engine start`. The outputs show container IDs, PIDs, and status messages, indicating that container metadata is being tracked.

When containers exit, messages like “exited container=alpha” appear, showing that the supervisor correctly detects termination and reaps processes. This ensures that no zombie processes remain. The logs also reflect whether a container exited normally or was terminated.

---

### 3. IPC and Logging
Two communication paths are observed from the outputs:

- CLI commands such as `engine start`, `engine ps`, and `engine stop` interact with the supervisor  
- Container output is captured and stored, as seen in the logs  

Pipes are used to redirect container stdout and stderr to the supervisor. The supervisor then writes this output to log files, ensuring that container output is preserved even after execution.

---

### 4. Memory Management and Enforcement
From the screenshots, containers are started with memory limits (soft and hard values). The kernel module is loaded and exposed through `/dev/container_monitor`.

Kernel logs (`dmesg`) show events when memory limits are exceeded. When the soft limit is crossed, a warning is generated. When the hard limit is exceeded, the container is terminated, which is also reflected in the supervisor output.

This shows that memory monitoring and enforcement are correctly handled in kernel space.

---

### 5. Scheduling Behavior
From the outputs, multiple containers are executed at the same time, demonstrating concurrent execution. This shows that the system allows multiple processes to run simultaneously and share CPU resources.

The observed behavior reflects how the Linux scheduler distributes CPU time among running processes, ensuring that all containers make progress.

---

## Design Decisions and Tradeoffs

- Using namespaces and separate rootfs provides isolation but shares the same kernel  
- Supervisor-based design simplifies control but acts as a central dependency  
- Pipe-based logging ensures reliable output capture  
- Kernel-based memory monitoring provides accurate enforcement but increases complexity  

---

## Scheduler Experiment Results

| Configuration | Observation |
|--------------|------------|
| Multiple containers running | CPU time is shared between containers |
| Concurrent execution | All containers continue to make progress |

This shows that the Linux scheduler distributes CPU time fairly among running processes.
