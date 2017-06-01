#include "mmu.h"


int init(void) {

    //defining page tables

    //root page table size: 16 kB
    //up to 4096 entries, each either a 1 MB section or a Pointer to a L2 PT
    //covers 4GB
    rootPT.vAddress = 0x00000000;
    rootPT.ptAddress = ROOT_PT_V_ADDRESS;
    rootPT.rootPTAddress = ROOT_PT_V_ADDRESS;
    rootPT.type = ROOT;
    rootPT.domain = 3; //reread what domain does...

    //coarse page table size: 1 kB
    //up to 256 entries, each covering a 4 kB memory
    //covers 1 MB
    systemPT.vAddress = 0x80000000;
    systemPT.ptAddress = 0x80094000;
    systemPT.rootPTAddress = ROOT_PT_V_ADDRESS;
    systemPT.type = COARSE;
    systemPT.domain = 3;

    /* no task yet ...
    task1PT.vAddress = 0x00000000;
    task1PT.ptAddress;
    task1PT.rootPTAddress = ROOT_PT_V_ADDRESS;
    task1PT.type = COARSE;
    task1PT.domain = 3;
    */


    //defining regions

    //kernel size: 512kB
    kernelRegion.vAddress = 0x8000000;
    kernelRegion.pageSize = 4;
    kernelRegion.numPages = 128;
    kernelRegion.AP;
    kernelRegion.CP;
    kernelRegion.pAddress = 0x8000000;
    kernelRegion.PT = &systemPT;

    //shared size: 64kB
    sharedRegion.vAddress = 0x80080000;
    sharedRegion.pageSize = 4;
    sharedRegion.numPages = 16;
    sharedRegion.AP;
    sharedRegion.CP;
    sharedRegion.pAddress = 0x80080000;
    sharedRegion.PT = &systemPT;

    //page table size: root PT + system PT = 16kb + 1kb -> 32kB
    PTRegion.vAddress = ROOT_PT_V_ADDRESS;
    PTRegion.pageSize = 4;
    PTRegion.numPages = 8;
    PTRegion.AP;
    PTRegion.CP;
    PTRegion.pAddress = ROOT_PT_V_ADDRESS;
    PTRegion.PT = &systemPT;


    //init page tables
    if (!mmuInitPT(rootPT) || !mmuInitPT(systemPT)) {
        return -1;
    }

}


int mmuInitPT(page_table_t *pt) {
    unsigned int* pPTBase = (unsigned int*) pt->ptAddress;
    unsigned int PTEntry = FAULT; // = 0

    int index;
    switch (pt->type) {
        case ROOT:
            index = 4096; //number of entries in the root PT
            break;
        case COARSE:
            index = 256; // number of entries in a coarse PT
            break;
        default:
            return -1;
    }

    int i = 0;
    while (i < index) {
        *pPTBase = PTEntry;     //set content of pointer to FAULT
        ++pPTBase;              //increase Pointer by one unsigned int to get the next pointer. POINTER ARITHMETICS YO
        ++i;
    }

    return 1;
}
