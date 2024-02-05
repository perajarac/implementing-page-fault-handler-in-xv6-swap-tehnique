#ifndef XV6_RISCV_OS2_RISCV_VM_SWAP_SWAP_H
#define XV6_RISCV_OS2_RISCV_VM_SWAP_SWAP_H
#include "defs.h"
#include "memlayout.h"

#define TOTALMEM (PHYSTOP-KERNBASE)
#define BLOCKS (1<<24/(PGSIZE>>2)) //16MB swap space /  1024 B

 uint8 blocks[BLOCKS];

void initblockvector();

//static uint64 ref_vector[4096];

 uint8 rw;


uint32 pageFaultAlloc(struct proc* process, uint64 va);



#endif
