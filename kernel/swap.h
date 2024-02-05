#ifndef XV6_RISCV_OS2_RISCV_VM_SWAP_SWAP_H
#define XV6_RISCV_OS2_RISCV_VM_SWAP_SWAP_H
#include "param.h"
#include "types.h"
#include "memlayout.h"
#include "elf.h"
#include "riscv.h"
#include "defs.h"
#include "fs.h"
#include "spinlock.h"
#include "proc.h"
#include "virtio.h"
#include "swap.h"

uint64 ref_vector[4096];

static uint8 rw;


uint32 pageFaultAlloc(struct proc* process, uint64 va);



#endif
