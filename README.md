# MODIFYING XV6-RISCV

## Aditya Tejpaul | 2021111025

***

## Overview

Several modifications and improvements have been implemented into XV6 as a part of this project.
Run xv6 using the command: `make clean && make qemu SCHEDULER=<scheduler_name>` where `<scheduler_name>` can be one of the following: `DEFAULT`, `FCFS`, `PRIORITY`, `LOTTERY`. Not specifying a scheduler will default to `DEFAULT`.

## Specification 1a: `trace` system call

- Running `strace mask command [args]` prints all the system calls used by the command which have been specified by the `mask` argument. 

### Steps:
1. Define the `trace` system call in syscall.c, syscall.h, usys.pl, user.h, and sysproc.c by doing the necessary additions.
2. Make an array `char *syscall_names[]` which stores the names of every system call to be printed as the output.
3. A flag variable `int mask_bit` is added to `struct proc` in proc.h to store the bits corresponding to the system calls specified by the `mask` argument.
4. Save the arguments (from the trapframe) in the registers into `int args[]` since the return value of the system call will be overwritten into `a0` register. Also, store the number of arguments of every system call into `int hardcoded_args[]` to be used in the next step.
5. In the `void syscall(void)` function in syscall.c, print the required info of a system call if its bits and the `mask` argument's bits match.

## Specification 1b: `sigalarm` and `sigreturn` system calls

- `sigalarm` sends interrupts to processes after a specified amount of time, and `sigreturn` resets the process state to what it was before the `sigalarm` system call was executed.

### Steps:
1. Declare the `sigalarm` and `sigreturn` system calls in syscall.c, and syscall.h by doing the necessary additions.
2. Add additional variables in `struct proc` to keep tracks of how many ticks have occured, to store the number of ticks specified by the user, to store the address of handler function of `sigalarm`, and to store the trapframe for `sigreturn`.
3. Modify `allocproc()` and `freeproc()` in proc.c to initialize and modify the newly added variables at process creation and while freeing it.
4. Define `sys_sigalarm()` and `sys_sigreturn()` in sysproc.c to set the variables accordingly. Additionaly, use the duplicated trapframe in to restore the original trapframe of the process in `sys_sigreturn()`.
5. Modify `trap()` in trap.c to handle the `SIGALRM` interrupt and to call the handler function of `sigalarm` if it is set.

*** 

## Specification 2a: FCFS Scheduling 

Following changes were made to the xv6 codebase to allow for FCFS-based scheduling:

* <i> struct proc </i> in <i> proc.h </i> has been edited to store the creation time of a process.

* Changes were made to the <i>allocproc()</i> function in <i> proc.c </i> to initialize the variable <i> cretime </i>.

* <i> scheduler() </i> in <i> proc.c </i> has been edited to allow for FCFS scheduling. This segment of the function is defined only when the user chooses FCFS at compile time. The FCFS works by choosing the process with the lowest value of the variable <i> cretime </i>.

## Specification 2b: Priority Scheduling

Following changes were made to the xv6 codebase to allow for Priority-based scheduling:

* <i> struct proc </i> in <i> proc.h </i> has been edited to store the static priority assigned to each process as well as the niceness of a process.

* Changes were made to the <i>allocproc()</i> function in <i> proc.c </i> to initialize the variable <i> niceness </i> and <i> priority</i>. By default each process is assigned 5 niceness and 60 static priority.

* Changes were subsequently made to <i> syscall.h, syscall.c, sysproc.c, user.h </i> and finally <i> usys.pl </i> with the intention of defining a system call <i> set_priority() </i> that can be called through the command line to both assign a process a different static priority as well as to reset its niceness.

* <i> scheduler() </i> in <i> proc.c </i> has been edited to allow for priority based scheduling. This segment of the function is defined only when the user chooses priority based scheduling at compile time. The priority based scheduler works by calculating the dynamic priority based on static priority and niceness. We use the number of schedulings of a process and then the creation time of a process to break any ties.

## Specification 2c: Lottery Scheduling
Following changes were made to the xv6 codebase to allow for lottery based scheduling:

* <i> struct proc </i> in <i> proc.h </i> has been edited to store the number of tickets assigned to each process. 

* Changes were made to the <i>allocproc()</i> function in <i> proc.c </i> to initialize the variable <i> tickets </i>. By default each process is assigned 1 ticket.

* Changes were subsequently made to <i> syscall.h, syscall.c, sysproc.c, user.h </i> and finally <i> usys.pl </i> with the intention of defining a system call <i> set_tickets() </i> that can be called by any process to assign it more tickets. Each process is free to ask for more tickets to increase the timeslice alloted to it by the scheduler.

* <i> scheduler() </i> in <i> proc.c </i> has been edited to allow for lottery based scheduling. This segment of the function is defined only when the user chooses lottery based scheduling at compile time. The lottery based scheduler works by iterating over all the processes to compute the total number of tickets assigned. It then generates a random number somewhere between 1 and <i> total_tickets </i>. Finally, it iterates over all the processes again and maintains a cumulative sum of tickets, choosing a process as soon as this cumulative sum becomes equal to or greater than the number randomly chosen by the scheduler.


***

## Specification 3: Copy-on-Write Fork

- COW Fork results in the child process sharing the same physical memory as the parent process until the child process tries to write to the memory (which is kept read-only). At this point, the child process is given a copy of the memory page and the parent process is given a copy-on-write page.

### Steps:
1. Start by modifying `uvmcopy()` function in vm.c to copy the parent's page table to the child's page table. This is done by using `mappages()` to map the page table entry of the parent to the child.
2. `int page_fault_handler()` in trap.c is made to handle the page fault exception. If the page fault is due to a write access, then the page is copied to a new page and the parent is given a copy-on-write page. 
3. Modify `usertrap()` function to call `page_fault_handler()` if the exception is due to a page fault.
4. Make functions `init_page_ref()`, `dec_page_ref`, `inc_page_ref`, and `get_page_ref` in kalloc.c to modify the reference count of a page in the newly made `struct page_ref`. 
5. Modify `kalloc()` and `kfree()` in kalloc.c to increment and decrement the reference count of a page respectively.
6. Modify `copyout()` in vm.c to also handle page faults involving COW pages using `page_fault_handler()`.
