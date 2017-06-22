#include "mmu.h"
/*
 * NA = no access, RO = read only, RW = read/write
 * First part is system, second part is user access permissions
 */
typedef enum
{
    NANA = 0x0, RWNA = 0x1, RWRO = 0x2, RWRW = 0x3
} rr_permissions_t;

/*
 * Cache and Buffer settings
 * cb = not cached/not buffered
 * cB = not Cached/Buffered
 * Cb = Cached/not Buffered
 * WT = write through cache
 *
 */
typedef enum
{
    cb = 0x0, cB = 0x1, Cb = 0x2, WT = 0x3
} cache_settings_t;

#define  ROOT_PT_V_ADDRESS      0x80090000
#define INTVECS_BASE_ADDRESS    0x4020FFC0

/*****************
 *  Page Tables  *
 *****************/
typedef enum{
    domain1=1,domain2=2,domain3=3
}domain_t;
/*page_table_t c = {vAddress, ptAddress, rootPTAddress, type, domain}*/
/**
 * Root Page Table:
 *  -size: 16 kB
 *  -up to 4096 entries, each either a 1 MB section or a Pointer to a L2 PT
 *  -covers 4GB
 */
page_table_t rootPT = { 0x00000000, ROOT_PT_V_ADDRESS, ROOT_PT_V_ADDRESS, ROOT,
                        domain3 };
/**
 * Coarse Page Table:
 *  -size: 1kB
 *  -up to 256 entries, each covering a 4kb memory
 *  -covers 1MB
 */
page_table_t systemPT = { 0x80000000, 0x80094000, ROOT_PT_V_ADDRESS, COARSE, domain3 };

page_table_t *taskPTs[16];

/*****************
 * Regions       *
 *****************/
typedef enum{
    page_size_section = 1024, page_size_tiny = 1, page_size_small = 4, page_size_large =64
};
/*regio_t x = {vAddress, pageSize, numPages, AP, CB, pAddress, PT}*/
/*kernel size: 512kB*/
region_t kernelRegion = { 0x80000000, page_size_small, 128, RWRW, WT, 0x80000000, &systemPT };
/*shared size: 64kB*/
region_t sharedRegion = { 0x80080000, page_size_small, 16, RWRW, WT, 0x80080000, &systemPT };
/*page table size: root PT + system PT = 16kb + 1kb -> 32kB*/
region_t PTRegion = { ROOT_PT_V_ADDRESS, page_size_small, 8, RWRW, WT, ROOT_PT_V_ADDRESS,
                      &systemPT };
region_t peripheralRegion = { 0x40000000, page_size_section, 1024, RWRW, WT, 0x40000000,
                              &rootPT };
region_t bootRegion = { 0x00000000, page_size_section, 1024, RWRW, WT, 0x00000000, &rootPT };
region_t taskRegion = { 0x80494000, page_size_small, 256, RWRW, WT, 0x80494000, &task1PT };

int mmu_init(void)
{

    //create 16 page tables and task regions for a maximum of 16 tasks
    volatile int i;
    for (i = 0; i < 16; ++i)
    {
        mmu_create_task_PT_and_region(i);
    }

    //init page tables
    if (!mmuInitPT(&rootPT) || !mmuInitPT(&systemPT))
    {
        return -1;
    }

    //map fixed regions
    if (!mmuMapRegion(&kernelRegion) || !mmuMapRegion(&sharedRegion)
            || !mmuMapRegion(&PTRegion) || !mmuMapRegion(&peripheralRegion)
            || !mmuMapRegion(&bootRegion))
    {
        return -1;
    }

    //set root PT as Translation Table Base (TTB) -> the PT in which the MMU searches after it looks in the TLB
    mmuAttachPT(&rootPT);
    mmuAttachPT(&systemPT);
    mmuAttachPT(taskPTs[0]); //set page table of first task

    //set all domains to 3 which means they all have equal domain access
    //active access permissions are set in regions and thus in pages
    set_domain();

    //set mmu control register
    set_intvecs_base_address((unsigned int *) INTVECS_BASE_ADDRESS);
    mmu_flush_tlb();
    mmu_flush_cache();
    set_mmu_config_register_and_enable_mmu();
    return 1;
}

int mmuInitPT(page_table_t *pt)
{
    unsigned int* pPTEntry = (unsigned int*) pt->ptAddress; // base address of page table = first PT entry
    unsigned int PTEntryValue = FAULT; // = 0                    // value which will be used for each entry

    int index;
    switch (pt->type)
    {
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
    while (i < index)
    {
        *pPTEntry = PTEntryValue;     //set content of pointer to FAULT
        ++pPTEntry; //increase Pointer by one unsigned int to get the next pointer. POINTER ARITHMETICS YO
        ++i;
    }
    return 1;
}

int mmuMapRegion(region_t *region)
{
    switch (region->PT->type)
    {
    case ROOT:
        return mmuMapSectionTableRegion(region);
    case COARSE:
        return mmuMapCoarseTableRegion(region);
    default:
        return -1;
    }
}

int mmuMapSectionTableRegion(region_t *region)
{

    unsigned int PTEValue;
    unsigned int* pPTE = (unsigned int*) region->PT->ptAddress; //base address of page table = first PT entry

    pPTE += region->vAddress >> 20;

    PTEValue = region->pAddress & 0xFFF00000;           // set physical address
    PTEValue |= (region->AP & 0x3) << 10;              // set Access Permissions
    PTEValue |= region->PT->domain << 5;               // set Domain for section
    PTEValue |= (region->CB & 0x3) << 2;  // set Cache and Write Back attributes
    PTEValue |= 0x2;                                   // set as section entry

    int i;
    for (i = 0; i < region->numPages; ++i)
    {
        *pPTE = PTEValue + (i << 20);                // i as index, always +1 MB
        ++pPTE;
    }

    return 1;
}

int mmuMapCoarseTableRegion(region_t *region)
{

    unsigned int PTEntryValue;                 //value that is set to each entry
    unsigned int* pPTE = (unsigned int*) region->PT->ptAddress; //base address of page table = first PT entry
    unsigned int accessPermissions = region->AP & 0x3;

    //NOTE: if we were ever to add large pages we would have catch it here and change the code accordingly
    pPTE += (region->vAddress & 0x000FF000) >> 12;

    PTEntryValue = region->pAddress & 0xFFFFF000; // the first 20 bits of the PT entry are the physical address base
    PTEntryValue |= accessPermissions << 10;     // access permissions subpage 3
    PTEntryValue |= accessPermissions << 8;                  // ap subpage 2
    PTEntryValue |= accessPermissions << 6;                  // ap subpage 1
    PTEntryValue |= accessPermissions << 4;                  // ap subpage 0
    PTEntryValue |= (region->CB & 0x3) << 2; // set cache and write back attributes
    PTEntryValue |= 0x1;                              // set as small page (4kB)

    int i;
    for (i = 0; i < region->numPages; ++i)
    {
        *pPTE = PTEntryValue + (i << 12);            // i as index, always +4 kB
        ++pPTE;
    }
    return 1;
}

int mmuAttachPT(page_table_t *pPT)
{

    unsigned int* pTTB = (unsigned int*) pPT->rootPTAddress; // get root PT baseaddress
    unsigned int offset;
    unsigned int PTE;

    switch (pPT->type)
    {
    case ROOT:
        set_root_pt_register(pTTB);
        break;
    case COARSE:
        offset = (pPT->vAddress) >> 20; // find the offset in which this coarse PT resides
        PTE = (pPT->ptAddress & 0xFFFFFc00); // L1 entry of a coarse PT takes 22 bits as base address
        PTE |= pPT->domain << 5;                    // set domain
        PTE |= 0x11;                                // set as coarse PT entry
        pTTB[offset] = PTE;                // write the new PTE into the root PT
        break;
    default:
        //unknown PT type
        return -1;
    }
    return 1;
}

void mmu_create_task_PT_and_region(int proc_id)
{

    page_table_t taskPT;

    taskPT.vAddress = 0x80494000;
    taskPT.ptAddress = 0x80094400 + (0x400 * proc_id);
    taskPT.rootPTAddress = ROOT_PT_V_ADDRESS;
    taskPT.type = COARSE;
    taskPT.domain = 3;

    mmuInitPT(&taskPT);
    taskPTs[proc_id] = &taskPT;
    create_task_region(&taskPT, proc_id);
}

void create_task_region(page_table_t *pTaskPT, int proc_id)
{
    region_t taskRegion;

    taskRegion.vAddress = 0x80494000;
    taskRegion.pageSize = 4;
    taskRegion.numPages = 256;
    taskRegion.AP = RWRW;
    taskRegion.CB = WT;
    taskRegion.pAddress = 0x80494000 + (0x100000 * proc_id);
    taskRegion.PT = pTaskPT;

    mmuMapRegion(&taskRegion);
}

