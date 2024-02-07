#define KERNB 0x80000000L
#define PHYS (KERNB + 16*1024*1024)
#define NULL 0
#define INDEX(pa) (((uint64)pa-(uint64)KERNB)/4096)
#define MAXINT 0xffffffff

#define TOTALMEM (PHYS-KERNB)
#define BLOCKS (1<<12) //16MB swap space /  1024 B  2^24 / 2 ^ 10 / 2^2

typedef struct indtpte{   //mapiranje indeksa iz tabele referenciranja u pte
    uint32 refbits;
    pte_t* pte;
    int mode;
}indtpte;

extern indtpte map[4096];

extern uint8 blocks[BLOCKS];

extern uint8 rw;

void initblockvector();

int getBlock();

pte_t* getVictim();

int pageFaultAlloc(struct proc* process, uint64 va);

void updateRefBits();

void freeBlock(uint64 index);

uint64 readFromDisk(int);