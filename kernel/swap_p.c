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
    int first_block = getBlock();
    if(first_block == -1) return -2;
    if(readFromDisk(page_entry,first_block)<0) return -1;

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

void freeBlock(uint64 index){
    blocks[index] = 0;
}

int readFromDisk(pte_t* pte, int block){
    char* mem;
    if ((mem = kalloc()) == 0) {
        printf("usertrap(): kalloc failed\n");
        return -1;
    }
    rw = 1;

    int first_block = (*pte >> 9)<<2;
    if(block!=0) first_block = block;
    for(int i = 0; i<4 ;i++){
        read_block(first_block++,(uchar*)((uint64)mem+i*PGSIZE/4),1);
    }
    rw = 0;

    *pte  = (PA2PTE((uint64)mem) | PTE_FLAGS(*pte) | PTE_V) & (~PTE_UP);

    return 0;
}
