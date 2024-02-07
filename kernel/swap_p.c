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

    int first_block = (*page_entry >> 10);
    if(first_block == -1) return -2;

    uint64 mem = readFromDisk(first_block);
    if(mem == 0) return -1;

    *page_entry  = (PA2PTE((uint64)mem) | PTE_FLAGS(*page_entry) | PTE_V) & (~PTE_UP);


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

        map[i].refbits >>= 1;

        if(flags & PTE_A){
            *map[i].pte &= (~PTE_A);
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

void freeBlock(uint64 index){
    blocks[index] = 0;
}

uint64 readFromDisk(int block){
    char* mem;
    int first_block = block;
    if ((mem = kalloc()) == 0) {
        printf("usertrap(): kalloc failed\n");
        return 0;
    }
    rw = 1;


    for(int i = 0; i<4 ;i++){
        read_block(4*first_block+i,(uchar*)((uint64)mem+i*PGSIZE/4),1);
    }
    rw = 0;

    return (uint64)mem;
}
