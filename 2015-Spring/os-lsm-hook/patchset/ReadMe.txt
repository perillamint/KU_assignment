!!! Note:
TO COMPILE, PLEASE USE PATCH FILE (os2_2013210101.patch) INSTEAD OF TARBALL
Tarball file *may* fail to build since it does not record diff.

System call files
linux-2.6.35.6/2nd_assign/Makefile
linux-2.6.35.6/2nd_assign/main.c

Main makefile
linux-2.6.35.6/Makefile

System call headers and tables
linux-2.6.35.6/arch/x86/include/asm/unistd_32.h
linux-2.6.35.6/arch/x86/include/asm/unistd_64.h
linux-2.6.35.6/arch/x86/kernel/syscall_table_32.S

task_struct modification
linux-2.6.35.6/include/linux/init_task.h
linux-2.6.35.6/include/linux/sched.h
linux-2.6.35.6/include/linux/syscalls.h

SELinux hook
linux-2.6.35.6/security/selinux/hooks.c
