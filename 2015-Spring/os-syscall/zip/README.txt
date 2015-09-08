./my_queue_syscall.patch
Patch file generated using git format-patch.

./1st_assign/Makefile
Makefile to build system call.

./1st_assign/main.c
sys_my_enqueue, sys_my_dequeue implementation.

./arch/x86/include/asm/unistd_32.h
Defines system call numbers in x86 environment.
Patched to add __NR_my_enqueue and __NR_my_dequeue.

./arch/x86/include/asm/unistd_64.h
Defines system call numbers in AMD64 environment.
Patched to add __NR_my_enqueue and __NR_my_dequeue.

./arch/x86/kernel/syscall_table_32.S
Syscall table entry - x86 environment.

./Makefile
Linux kernel main makefile. patched to include 1st_assign subdirectory.

./include/linux/syscalls.h
Prototype of system call functions.
Patched to include sys_my_enqueue and sys_my_dequeue.

./README.txt
This file.
