# Operating Systems Project – System Call Wrappers and Kernel Extensions

**Course:** CS3500  
**Group No.:** 3  
---

## 📚 Project Overview

This repository contains the source code and documentation for our CS3500 Operating Systems project.  
The core focus is on creating **system call wrappers** and **kernel-level customizations** for advanced resource management, security, and debugging in Linux systems.

Our work is divided into three main areas:

- **File I/O Wrappers**  
- **Memory Management Wrappers**  
- **Process Management and Kernel Extensions**

---

## 🧰 Features & Implemented Wrappers

### File I/O Wrappers

- **Logger:** Intercepts file operations, logs actions and results for debugging and monitoring.
- **Safe Read:** Validates file read operations, prevents out-of-bound reads, robust error handling.
- **Safe Open:** Checks file existence, validates flags, and prevents typical open errors.
- **Access Control:** Enforces user permission checks before file access.
- **Rate Limiting:** Limits frequency of file operations to prevent abuse or DoS attacks.
- **Buffering:** Retains data in-memory before writing for performance boosts.
- **Master Wrapper:** Uses `LD_PRELOAD` to transparently intercept all major file syscalls.

### Memory Management Wrappers

- **mmap Allocation Tracking:** Linked-list tracking for all memory allocation events and error logging.
- **Safe munmap:** Checks validity before unmapping, detailed error reporting.
- **Leak Detection:** Automatic logging and detection of dynamic memory leaks.
- **Thread-safe brk:** Ensures robust heap management across threads, falls back to mmap when necessary.
- **File Descriptor Management:** Tracks open descriptors, prevents leaks/zombies.
- **Memory Pool:** Preallocates a pool for efficient frequent small allocations.
- **Aligned Allocation:** Ensures hardware- or SIMD-compatible memory address alignment.

### Process and Kernel Wrappers

- **Process Pool Manager:** Efficiently manages tasks across child processes, reusing them to maximize CPU utilization.
- **Deadlock Detection:** Dynamically builds a process-resource graph, detects cycles, resolves by killing the latest process.
- **Process Cloaking:** Hides chosen child processes from listing utilities (`ps`, `top`).
- **Wait-for-All-Children:** Blocks parent until all child processes exit, preventing zombies.
- **IPC Logging:** Logs and audits all major IPC operations (pipes, queues, shared memory, semaphores).
- **Priority Enforcement:** Sets CPU and IO priorities post-fork, ensuring predictable resource usage.
- **Process Monitoring:** Tracks run stats (time, memory, context switches), logs abnormal events.
- **Custom waitpid:** Adds timeouts and non-blocking monitoring for child processes.
- **Zombie Logger:** Logs all zombie events upon kill/waitpid.
- **Custom Kernel Syscalls:** Examples include `hello` syscall, resource management calls like disabling fork, closing all files, etc.

---


### Kernel-Space Extensions

1. Download and extract Linux kernel (e.g., 6.0.7).
2. Add/modify syscall or kernel module source code in `kernel_compilation/`.
3. Update `syscalls.h`, syscall tables, and `Makefile` as needed.
4. Recompile and install kernel:
    ```
    make menuconfig    # Configure kernel, or copy old .config
    make -jn           # Build (n = CPU cores for speed)
    make modules_install
    make install
    ```
5. Reboot and select the new kernel in GRUB.

### Example Usage

- **File wrappers:**  

LD_PRELOAD=./filewrp.so cat myfile.txt

text
- **Memory leak checker:**  
Run with memory wrapper preload, check terminal/logs for leaks after execution.

- **Kernel syscalls:**  
Write and compile a minimal C program using your custom syscall number, e.g.,

syscall(549); // fork_if_not_disable

text

---

## 📝 Customization

- **Access Control Policy:** Edit `check_permission` in `control_permission.c`
- **Rate Limiting:** Set constants in `rate_limiting.h`
- **Buffer Size:** Update `BUFFER_SIZE` in `buffer.h`
- **Log File Location:** Set `LOG_FILE` in `logger.h`
- **Kernel Compilation:** Carefully follow instructions for your kernel version!

---

## ⚠️ Notes and Troubleshooting

- Some kernel versions have different syscall interfaces; update code and Makefile accordingly.
- Compilation times for the full kernel are long (often >2 hours).
- Bugs like frame buffer errors or system call registration issues may occur; refer to the referenced resources for fixes.

---

## 📖 References

- [PhoenixNAP: Build Linux Kernel](https://phoenixnap.com/kb/build-linux-kernel)
- [Medium: Adding Hello World System Call](https://medium.com/anubhav-shrimal/adding-a-hello-world-system-call-to-linux-kernel-dad32875872)
- [Medium: Custom Syscall on Linux 6.x](https://medium.com/@aryan20/create-custom-system-call-on-linux-6-8-126edef6caaf)
- Kernel.org: [Linux 6.x Release](https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.0.7.tar.xz)

---

## 💡 Additional Info

- Many wrappers use `LD_PRELOAD` with `dlsym()` for user-space syscall interception.
- Kernel wrappers use updated APIs (`filp_open`, `vfs_read`, etc.)—be careful with direct syscall invocation in kernel space due to security constraints.
- Process wrappers include robust resource tracking and logging to aid debugging/monitoring.
- All work was tested on Ubuntu 22.04.5; see notes for compatibility issues.

---
