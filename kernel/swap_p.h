#define KERNB 0x80000000L
#define PHYS (KERNB + 16*1024*1024)

#define TOTALMEM (PHYS-KERNB)
#define BLOCKS (1<<24/(4096>>2)) //16MB swap space /  1024 B

 extern uint8 blocks[BLOCKS];


//extern uint64 ref_vector[4096];

 extern uint8 rw;

void initblockvector();


uint32 pageFaultAlloc(struct proc* process, uint64 va);


