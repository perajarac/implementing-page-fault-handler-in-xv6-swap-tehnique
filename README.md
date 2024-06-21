# Introduction

<p align = "center">The goal of this task is to implement a part of the virtual memory system. The virtual memory system should provide page swapping . 
Multiple processes that share physical memory can exist in the system.  It is necessary to enable page swapping by transferring pages to a hard disk. Functions for accessing disk blocks are provided for accessing the swap space on the hard disk.
</p>

## Page swaping

### Introduction
<p align = "center>To record the virtual address space, the kernel of the operating system uses the hardware one
support for page organization of virtual memory. The page table is organized into three
level. The mapping method should be the same as in the xv6 operating system. At the beginning
operation, the system will be assigned one processor, physical memory of a fixed size and space on
fixed size replacement disk. Multiple processes are executed in the system, so it is necessary to ensure
adequate synchronization. The size of the virtual address is 39 bits, the size of the page is 4KB, while the
block size on disk 1KB. In the page table for each page there are bits for access rights,
a dirty bit, a referencing bit, and two bits that the software can use for its own purposes. </p>


