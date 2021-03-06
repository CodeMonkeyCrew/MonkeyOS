#ifndef KERNEL_MEMORYMANAGEMENT_MMU_H_
#define KERNEL_MEMORYMANAGEMENT_MMU_H_

#include "mm_type.h"

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

/*
 * sets the access bits of all domains
int setDomainAccess(unsigned int value, unsigned int mask);
*/

/*
 * sets control bits of the mmu

int setMMURegister(unsigned int value, unsigned int mask);
*/

/*
 * assembler function to set the root pagetable, needs to clear mask.
 * for TTRB0 it's needed to clear bits [13:0] to create the proper translation table base
 */
void set_root_pt_register(unsigned int* pRootPT, unsigned int mask);


/*
 * assembler function to set the access permission to all domains
 */
void set_domain(void);

/*
 * assembler function set intvects base address for mmu
 */
void set_intvecs_base_address(unsigned int* pIntvecsBaseAddress);

/*
 * assembler function to flush instruction and data TLB. If not clears there may be overlapping virtual addresses
 */
void mmu_flush_tlb(void);

/*
 * assembler function to flush data and instruction cache
 */
void mmu_flush_cache(void);


/*
 * assembler function that sets enabled data and instruction cache, enables alignment checking and enables mmu
 */
void set_mmu_config_register_and_enable_mmu(void);


/*
 * assembler function to enable translation table walk for TTBR0
 */
void mmu_enable_translation_table_walk_ttbr0(void);

void mmu_create_task_PT_and_region(int proc_id);

void create_task_region(page_table_t *pTaskPT, int proc_id);

void mmu_load_task_region(int proc_id);

#endif /* KERNEL_MEMORYMANAGEMENT_MMU_H_ */
