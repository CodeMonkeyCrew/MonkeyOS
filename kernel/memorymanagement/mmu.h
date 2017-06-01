#ifndef KERNEL_MEMORYMANAGEMENT_MMU_H_
#define KERNEL_MEMORYMANAGEMENT_MMU_H_

#include "mm_type.h"

#define ROOT_PT_V_ADDRESS 0x80090000

//page tables
static page_table_t rootPT;
static page_table_t systemPT;

// for each task a new page table
static page_table_t task1PT;


//regions
static region_t kernelRegion;
static region_t sharedRegion;
static region_t PTRegion;


/*
 * Sets the attributes of each page table and region
 */
int mmu_init(void);


/*
 * Initializes the page table by filling it with FAULT entries
 */
int mmuInitPT(page_table_t *pt);



#endif /* KERNEL_MEMORYMANAGEMENT_MMU_H_ */
