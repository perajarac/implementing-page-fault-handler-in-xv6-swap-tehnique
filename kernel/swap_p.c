#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "swap_p.h"

uint8 rw = 0;
uint8 blocks[BLOCKS];
indtpte map[4096];

void initblockvector(){
    for(int i = 0;i<BLOCKS;i++){
        blocks[i] = 0;
    }
}

int pageFaultAlloc(struct proc* process, uint64 va){

    pte_t* page_entry = walk(process->pagetable, va,0);
    if(!page_entry)return -3;
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

    int first_block = getBlock()*4;
    if(first_block == -1) return -5; //no free block on disk
    for(int i = 0; i<4 ;i++){
        read_block(first_block++,(uchar*)((uint64)mem+i*PGSIZE/4),1);
    }

    rw = 0;

    //mapiram velicinu diska, koliko celih page moze da stane i vodim evidenicju o slobodnim i zauzetim blokovima na osnovu toga mora kontinualno da bude alociarana

    //posle ovoga u pte postavljamo flagove, to jest adresu i bitove, da prepisemo sve flegove sa diska, staivm v bit i dodam u evidenciju zauzete blokove


    return 0;
}

int getBlock(){

    for(int i = 0; i < BLOCKS;i++){
        if(blocks[i]==0){
            blocks[i] = 1;
            return i;
        }
    }
    return -1; //unvalid
}

void updateRefBits(){   //call it from timer interrupt
    for(int i = 0;i<4096;i++){
        if(map[i].pte == NULL) continue;

        uint flags = PTE_FLAGS(*map[i].pte);

        map[i].refbits >>= 1; //ignorisati u getVictim kernel stranice

        if(flags & PTE_A){
            *map[i].pte &= 0xbf;
            map[i].refbits |= 0x80000000;
        }
    }
}

pte_t* getVictim(){
    pte_t* victim = 0;
    uint32 minimum = MAXINT;
    for(int i = 0; i < 4096; i++){
        if(map[i].pte == NULL) continue;
        if(map[i].refbits < minimum){
            minimum = map[i].refbits;
            victim = map[i].pte;
        }
    }
    return victim;
}