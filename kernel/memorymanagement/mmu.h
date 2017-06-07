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


/*
 * Maps fixed regions into page tables
 */
int mmuMapRegion(region_t *region);


/*
 * Subroutine of mmuMapRegion for Sections (root page table)
 */
int mmuMapSectionTableRegion(region_t *region);


/*
 * Subroutine of mmuMapRegion for coarse page tables
 */
int mmuMapCoarseTableRegion(region_t *region);


/*
 *  Either activates an L1 root page table by placing its adress into the TTB
 *  (in the CP15:c2:0 register), or activates an L2 page table by placing its base address
 *  into an L1 Root page table entry.
 */
int mmuAttachPT (page_table_t *pPT);


#endif /* KERNEL_MEMORYMANAGEMENT_MMU_H_ */
