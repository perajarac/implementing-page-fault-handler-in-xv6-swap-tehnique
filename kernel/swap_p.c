#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "swap_p.h"

uint8 rw = 0;

void initblockvector(){
    (void)rw;
    for(int i = 0;i<BLOCKS;i++){
        blocks[i] = 0;
    }
}

uint64 getVictim(){
    return 0;
}

uint32 pageFaultAlloc(struct proc* process, uint64 va){

    pte_t* page_entry = walk(process->pagetable, va,0);
    if(!page_entry )return -3;
    if((*page_entry & PTE_U) == 0) return -4;


    if (va >= process->sz || va < PGROUNDDOWN(process->trapframe->sp)) {
        printf("usertrap(): va is higher than size or below the user stack pointer\n");
        return -1;
    }

    char* mem;
    if ((mem = kalloc()) == 0) {
        printf("usertrap(): kalloc failed\n");
        return -2;
    }

    rw = 1;

    for(int i = 0; i<4 ;i++){
        read_block(i,(uchar*)((uint64)mem+i*PGSIZE/4),1); //ispraviti
    }

    rw = 0;

    //mapiram velicinu diska, koliko celih page moze da stane i vodim evidenicju o slobodnim i zauzetim blokovima na osnovu toga mora kontinualno da bude alociarana

    //posle ovoga u pte postavljamo flagove, to jest adresu i bitove, da prepisemo sve flegove sa diska, staivm v bit i dodam u evidenciju zauzete blokove



    return 0;
}

