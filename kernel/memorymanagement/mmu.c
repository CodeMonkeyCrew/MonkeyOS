#include "mmu.h"

#define  ROOT_PT_V_ADDRESS      0x80090000
#define INTVECS_BASE_ADDRESS    0x4020FFC0

/*****************
 *  Page Tables  *
 *****************/

/*page_table_t c = {vAddress, ptAddress, rootPTAddress, type, domain}*/
/**
 * Root Page Table:
 *  -size: 16 kB
 *  -up to 4096 entries, each either a 1 MB section or a Pointer to a L2 PT
 *  -covers 4GB
 */
page_table_t rootPT = { 0x00000000, ROOT_PT_V_ADDRESS, ROOT_PT_V_ADDRESS, ROOT, domain3 };
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
}page_size_t;
/*regio_t x = {vAddress, pageSize, numPages, AP, CB, pAddress, PT}*/
/*kernel size: 512kB*/
region_t kernelRegion = { 0x80000000, page_size_small, 128, RWRW, WT, 0x80000000, &systemPT };
/*shared size: 64kB*/
region_t sharedRegion = { 0x80080000, page_size_small, 16, RWRW, WT, 0x80080000, &systemPT };
/*page table size: root PT + system PT = 16kb + 1kb -> 32kB*/
region_t PTRegion = { ROOT_PT_V_ADDRESS, page_size_small, 8, RWRW, WT, ROOT_PT_V_ADDRESS, &systemPT };
region_t peripheralRegion = { 0x40000000, page_size_section, 1024, RWRW, WT, 0x40000000, &rootPT };
region_t bootRegion = { 0x00000000, page_size_section, 1024, RWRW, WT, 0x00000000, &rootPT };
//region_t taskRegion = { 0x80494000, page_size_small, 256, RWRW, WT, 0x80494000, &task1PT };

int mmu_init(void)
{

    //create 16 page tables and task regions for a maximum of 16 tasks
    /*volatile int i;
    for (i = 0; i < 16; ++i)
    {
        mmu_create_task_PT_and_region(i);
    } */

    //init page tables with fault entries
    mmuInitPT(&rootPT);
    mmuInitPT(&systemPT);

    //fill page tables with proper entries
    mmuMapRegion(&bootRegion);
    mmuMapRegion(&peripheralRegion);
    mmuMapRegion(&kernelRegion);
    mmuMapRegion(&sharedRegion);
    mmuMapRegion(&PTRegion);

    mmuAttachPT(&rootPT);       // sets the first level page table (root/master PT) for the MMU
    mmuAttachPT(&systemPT);     // creates a first level page table entry and adds it to the root PT
    //mmuAttachPT(taskPTs[0]); //set page table of first task

    //set all domains to 3 which means they all have equal domain access
    //active access permissions are set in regions and thus in pages
    set_domain();

    set_intvecs_base_address((unsigned int *) INTVECS_BASE_ADDRESS);

    mmu_flush_tlb();
    mmu_flush_cache();

    mmu_enable_translation_table_walk_ttbr0();

    //enable alignment checking and MMU
    set_mmu_config_register_and_enable_mmu();
    return 1;
}

int mmuInitPT(page_table_t *pt)
{
    unsigned int* pPTEntry = (unsigned int*) pt->ptAddress;      // base address of page table = first PT entry
    unsigned int PTEntryValue = FAULT; // = 0                    // value which will be used for each entry

    int index;
    switch (pt->type)
    {
    case ROOT:
        index = 4096;   // number of entries in the root PT
        break;
    case COARSE:
        index = 256;    // number of entries in a coarse PT
        break;
    default:
        return -1;
    }

    int i = 0;
    while (i < index)
    {
        *pPTEntry = PTEntryValue;     //set content of pointer to FAULT
        ++pPTEntry;                   //increase Pointer by one unsigned int to get the next pointer. POINTER ARITHMETICS YO
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
    //see ARMv7 architecture reference manual on page B3-1335
    unsigned int* pPTE = (unsigned int*) region->PT->ptAddress; //base address of page table
    unsigned int index = region->vAddress >> 20;                //get the offset/index to the virtual memory for first entry address
    index = index << 2;                                         //add two zeros on the right side to fulfill the requirements for the pointer

    pPTE = (unsigned int*)((unsigned int)pPTE + index);         //add them and recast to pointer

    //first level descriptor found in ARMv7 architecture - p. B3-1326
    fld_section_t firstLevelSectionDescriptor;
    firstLevelSectionDescriptor.fld_raw = 0;
    firstLevelSectionDescriptor.fld_split.SBA = ((region->pAddress & 0xFFF00000) >> 20);      // set physical address; section base address is only 12 bits long so we shift right by 20
    firstLevelSectionDescriptor.fld_split.AP1_0 = region->AP & 0x3;                           // set Access Permissions
    firstLevelSectionDescriptor.fld_split.DOM = region->PT->domain;                           // set Domain for section
    firstLevelSectionDescriptor.fld_split.B = (region->CB & 0x1);                             // set Buffer/WriteBack attributes
    firstLevelSectionDescriptor.fld_split.C = ((region->CB & 0x2) >> 1);                      // set Cache attribute
    firstLevelSectionDescriptor.fld_split.TYPE = 0x2;                                         // set as section entry

    int i;
    for (i = 0; i < region->numPages; ++i)
    {
        unsigned int temp = firstLevelSectionDescriptor.fld_raw + (i << 20);                // i as index, always +1 MB
        *pPTE = temp;
        ++pPTE;
    }
    return 1;
}

int mmuMapCoarseTableRegion(region_t *region)
{
    //NOTE: if we were ever to add large pages we would have catch it here and change the code accordingly

    //see ARMv7 architecture reference manual on page B3-1337
    unsigned int* pPTE = (unsigned int*) region->PT->ptAddress;         //base address of page table = first PT entry
    unsigned int index = (region->vAddress & 0x000FF000) >> 12;         //get the offset/index to the virtual memory
    index = index << 2;                                                 //add two zeros on the right side to fulfill the requirements for the pointer
    pPTE = (unsigned int*)((unsigned int)pPTE + index);                 //add index and recast to pointer

    sld_small_page_t secondLevelSmallPageDescriptor;
    secondLevelSmallPageDescriptor.sld_raw = 0;
    secondLevelSmallPageDescriptor.sld_split.SPBA = ((region->pAddress & 0xFFFFF000) >> 12);    // set physical address
    secondLevelSmallPageDescriptor.sld_split.AP1_0 = region->AP & 0x3;                          // set Domain for section
    secondLevelSmallPageDescriptor.sld_split.B = (region->CB & 0x1);                            // set Buffer/WriteBack attributes
    secondLevelSmallPageDescriptor.sld_split.C = ((region->CB & 0x2) >> 1);                     // set Cache attribute, shift by one to get the [1]0
    secondLevelSmallPageDescriptor.sld_split.TYPE = 0x2;

    int i;
    for (i = 0; i < region->numPages; ++i)
    {
        unsigned int temp = secondLevelSmallPageDescriptor.sld_raw + (i << 12); // i as index, always +4 kB
        *pPTE = temp;
        ++pPTE;
    }
    return 1;
}

int mmuAttachPT(page_table_t *pPT)
{
    unsigned int* pTTB = (unsigned int*) pPT->rootPTAddress; // get root PT baseaddress
    unsigned int offset;
    fld_coarse_t fld_coarse;

    switch (pPT->type)
    {
    case ROOT:
        set_root_pt_register(pTTB, 0x3FFF);
        break;
    case COARSE:
        fld_coarse.fld_raw = 0;
        fld_coarse.fld_split.CPT = ((pPT->ptAddress & 0xFFFFFC00) >> 10);   // L1 entry of a coarse PT takes 22 bits as base address
        fld_coarse.fld_split.DOMAIN = pPT->domain;                          // set domain
        fld_coarse.fld_split.TYPE = 0b01;                                   // set as (coarse) page table

        offset = (pPT->vAddress) >> 20;                             // find the offset for the new coarse page table entry
        pTTB[offset] = fld_coarse.fld_raw;                          // write the new PTE into the root PT
        break;
    default:
        //unknown PT type
        return -1;
    }
    return 1;
}

void mmu_create_task_PT_and_region(int proc_id)
{
    unsigned int ptAddress = (0x80094400 + (0x400 * proc_id));
    page_table_t taskPT = {0x80494000, ptAddress, ROOT_PT_V_ADDRESS, COARSE, domain3};

    mmuInitPT(&taskPT);
    taskPTs[proc_id] = &taskPT;
    create_task_region(&taskPT, proc_id);
}

void create_task_region(page_table_t *pTaskPT, int proc_id)
{
    unsigned int pAddress = (0x80494000 + (0x100000 * proc_id));
    region_t taskRegion = {0x80494000,page_size_small, 256, RWRW, WT, pAddress, pTaskPT};
    mmuMapRegion(&taskRegion);
}


void mmu_switch_context(int PID) {
    mmu_flush_tlb();
    mmu_flush_cache();
    mmu_attach_pt(taskPTs[PID]);
}

