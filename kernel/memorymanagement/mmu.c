#include "mmu.h"

// NA = no access, RO = read only, RW = read/write
//First part is system, second part is user access permissions
#define NANA 0x0
#define RWNA 0x1
#define RWRO 0x2
#define RWRW 0x3

//#if defined(__TARGET_CPU_ARM720T)
    #define cb 0x0
    #define cB 0x1
    #define Cb 0x2
    #define WT 0x3
//#endif
/* cb = not cached/not buffered
 * cB = not Cached/Buffered
 * Cb = Cached/not Buffered
 * WT = write through cache
 */

#define DOMAIN3 0x00000040
#define CHANGEALLREGIONS 0xFFFFFFFF

#define ENABLEMMU 1
#define ENABLEALIGNING 2
#define ENABLEDCACHE 3
#define ENABLEICACHE 12

#define ROOT_PT_V_ADDRESS 0x80090000
#define INTVECS_BASE_ADDRESS 0x4020FFC0

int mmu_init(void) {

    //defining page tables

    //root page table size: 16 kB
    //up to 4096 entries, each either a 1 MB section or a Pointer to a L2 PT
    //covers 4GB
    rootPT.vAddress = 0x00000000;
    rootPT.ptAddress = ROOT_PT_V_ADDRESS;
    rootPT.rootPTAddress = ROOT_PT_V_ADDRESS;
    rootPT.type = ROOT;
    rootPT.domain = 3;

    //coarse page table size: 1 kB
    //up to 256 entries, each covering a 4 kB memory
    //covers 1 MB
    systemPT.vAddress = 0x80000000;
    systemPT.ptAddress = 0x80094000;
    systemPT.rootPTAddress = ROOT_PT_V_ADDRESS;
    systemPT.type = COARSE;
    systemPT.domain = 3;

    /* no task yet ... */
    task1PT.vAddress = 0x80494000;
    task1PT.ptAddress = 0x80094400;
    task1PT.rootPTAddress = ROOT_PT_V_ADDRESS;
    task1PT.type = COARSE;
    task1PT.domain = 3;



    //defining regions

    //kernel size: 512kB
    kernelRegion.vAddress = 0x8000000;
    kernelRegion.pageSize = 4;
    kernelRegion.numPages = 128;
    kernelRegion.AP = RWRW;
    kernelRegion.CB = WT;
    kernelRegion.pAddress = 0x8000000;
    kernelRegion.PT = &systemPT;

    //shared size: 64kB
    sharedRegion.vAddress = 0x80080000;
    sharedRegion.pageSize = 4;
    sharedRegion.numPages = 16;
    sharedRegion.AP = RWRW;
    sharedRegion.CB = WT;
    sharedRegion.pAddress = 0x80080000;
    sharedRegion.PT = &systemPT;

    //page table size: root PT + system PT = 16kb + 1kb -> 32kB
    PTRegion.vAddress = ROOT_PT_V_ADDRESS;
    PTRegion.pageSize = 4;
    PTRegion.numPages = 8;
    PTRegion.AP = RWRW;
    PTRegion.CB = WT;
    PTRegion.pAddress = ROOT_PT_V_ADDRESS;
    PTRegion.PT = &systemPT;

    peripheralRegion.vAddress = 0x40000000;
    peripheralRegion.pageSize = 1024;
    peripheralRegion.numPages = 1024;
    peripheralRegion.AP = RWRW;
    peripheralRegion.CB = WT;
    peripheralRegion.pAddress = 0x40000000;
    peripheralRegion.PT = &rootPT;

    bootRegion.vAddress = 0x00000000;
    bootRegion.pageSize = 1024;
    bootRegion.numPages = 1024;
    bootRegion.AP = RWRW;
    bootRegion.CB = WT;
    bootRegion.pAddress = 0x00000000;
    bootRegion.PT = &rootPT;

    taskRegion.vAddress = 0x80494000;
    taskRegion.pageSize = 4;
    taskRegion.numPages = 256;
    taskRegion.AP = RWRW;
    taskRegion.CB = WT;
    taskRegion.pAddress = 0x80494000;
    taskRegion.PT = &task1PT;

    //init page tables
    if (!mmuInitPT(&rootPT)     ||
        !mmuInitPT(&systemPT)   ||
        !mmuInitPT(&task1PT))
    {
        return -1;
    }

    //map fixed regions
    if (!mmuMapRegion(&kernelRegion)     ||
        !mmuMapRegion(&sharedRegion)     ||
        !mmuMapRegion(&PTRegion)         ||
        !mmuMapRegion(&peripheralRegion) ||
        !mmuMapRegion(&bootRegion)       ||
        !mmuMapRegion(&taskRegion))
    {
        return -1;
    }

    //set root PT as Translation Table Base (TTB) -> the PT in which the MMU searches after it looks in the TLB
    mmuAttachPT(&rootPT);
    mmuAttachPT(&systemPT);
    mmuAttachPT(&task1PT);


    //set all domains to 3 which means they all have equal domain access
    //active access permissions are set in regions and thus in pages
    set_domain();


    //set mmu control register
    set_intvecs_base_address((unsigned int *)INTVECS_BASE_ADDRESS);
    clear_tlb();
    set_mmu_config_register_and_enable_mmu();
    return 1;
}


int mmuInitPT(page_table_t *pt) {
    unsigned int* pPTEntry = (unsigned int*) pt->ptAddress;      // base address of page table = first PT entry
    unsigned int PTEntryValue = FAULT; // = 0                    // value which will be used for each entry

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
        *pPTEntry = PTEntryValue;     //set content of pointer to FAULT
        ++pPTEntry;                   //increase Pointer by one unsigned int to get the next pointer. POINTER ARITHMETICS YO
        ++i;
    }
    return 1;
}


int mmuMapRegion(region_t *region) {
    switch (region->PT->type) {
        case ROOT:
            return mmuMapSectionTableRegion(region);
        case COARSE:
            return mmuMapCoarseTableRegion(region);
        default:
            return -1;
    }
}

int mmuMapSectionTableRegion(region_t *region) {

    unsigned int PTEValue;
    unsigned int* pPTE = (unsigned int*)region->PT->ptAddress;          //base address of page table = first PT entry

    pPTE += region->vAddress >> 20;

    PTEValue = region->pAddress & 0xFFF00000;           // set physical address
    PTEValue |= (region->AP & 0x3) << 10;               // set Access Permissions
    PTEValue |= region->PT->domain << 5;                // set Domain for section
    PTEValue |= (region->CB & 0x3) << 2;                // set Cache and Write Back attributes
    PTEValue |= 0x2;                                   // set as section entry

    int i;
    for (i = 0; i < region->numPages; ++i) {
        *pPTE = PTEValue + (i << 20);                   // i as index, always +1 MB
        ++pPTE;
    }

    return 1;
}


int mmuMapCoarseTableRegion(region_t *region) {

    unsigned int PTEntryValue;                                          //value that is set to each entry
    unsigned int* pPTE = (unsigned int*)region->PT->ptAddress;          //base address of page table = first PT entry
    unsigned int accessPermissions = region->AP & 0x3;

    //NOTE: if we were ever to add large pages we would have catch it here and change the code accordingly
    pPTE += (region->vAddress & 0x000FF000) >> 12;

    PTEntryValue = region->pAddress & 0xFFFFF000;            // the first 20 bits of the PT entry are the physical address base
    PTEntryValue |= accessPermissions << 10;                 // access permissions subpage 3
    PTEntryValue |= accessPermissions << 8;                  // ap subpage 2
    PTEntryValue |= accessPermissions << 6;                  // ap subpage 1
    PTEntryValue |= accessPermissions << 4;                  // ap subpage 0
    PTEntryValue |= (region->CB & 0x3) << 2;                 // set cache and write back attributes
    PTEntryValue |= 0x1;                                     // set as small page (4kB)

    int i;
    for (i = 0; i < region->numPages; ++i) {
        *pPTE = PTEntryValue + (i << 12);                    // i as index, always +4 kB
        ++pPTE;
    }
    return 1;
}


int mmuAttachPT (page_table_t *pPT) {

    unsigned int* pTTB = (unsigned int*) pPT->rootPTAddress;    // get root PT baseaddress
    unsigned int offset;
    unsigned int PTE;

    switch (pPT->type) {
        case ROOT:
            //code from book that doesn't quite work:
            //__asm(" MCR p15, 0, pTTB, c2, c0, 0 ") ;
            //source: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.faqs/ka11865.html
            //register unsigned int mmuTTBRegister __asm("p15:#0:c2:c0:#0");      // write root PT baseaddress to the register so the MMU knows where it lies
            //mmuTTBRegister &= ~mmuTTBRegister; //clear bits
            //mmuTTBRegister |= pTTB;

            set_root_pt_register(pTTB);

            break;
        case COARSE:
            offset = (pPT->vAddress) >> 20;             // find the offset in which this coarse PT resides
            PTE = (pPT->ptAddress & 0xFFFFFc00);        // L1 entry of a coarse PT takes 22 bits as base address
            PTE |= pPT->domain << 5;                    // set domain
            PTE |= 0x11;                                // set as coarse PT entry
            pTTB[offset] = PTE;                         // write the new PTE into the root PT
            break;
        default:
            //unknown PT type
            return -1;
    }
    return 1;
}

/*
int setDomainAccess(unsigned int value, unsigned int mask) {
    register unsigned int domainRegister;

    asm("   MRC p15, #0, r0, c3, c0, #0");          //read the register
    domainRegister &= ~mask;                        //clear bits that change
    domainRegister |= value;                        //set bits that change
    asm("   MCR p15, #0, r0, c3, c0, #0");          //set domain register

    return 1;
}


int setMMURegister(unsigned int value, unsigned int mask) {
    register unsigned int mmuControlRegister;

    //asm("   MRC p15, #0, mmuControlRegister, c1, c0, #0");        //read the register
    //mmuControlRegister &= ~mask;                                    //clear bits that change
    //mmuControlRegister |= value;                                    //set bits that change
    //asm("   MCR p15, #0, mmuControlRegister, c1, c0, #0");        //set mmu control register

    return 1;
}
*/








