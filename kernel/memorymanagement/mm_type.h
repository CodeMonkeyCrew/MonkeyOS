#ifndef KERNEL_MEMORYMANAGEMENT_MM_TYPE_H_
#define KERNEL_MEMORYMANAGEMENT_MM_TYPE_H_

/*
 * see arm system developers guide, chapter 14
 * NOTE: Root Page Table = Master Page Table
 */

typedef enum {FAULT, COARSE, ROOT} page_table_type;

typedef struct {
    unsigned int vAddress;          // identifies the starting address of a 1 MB section of virtual memory controlled by either a section entry or an L2 page table.
    unsigned int ptAddress;         // is the address where the page table is located in virtual memory
    unsigned int rootPTAddress;     // is the address of the master L1 (root) page table. If it is the root PT the value is the same as ptAddress
    page_table_type type;           // type of the PT
    unsigned int domain;            // sets the domain assigned to the 1 MB memory blocks of an L1 table entry
} page_table_t;


typedef struct {
    unsigned int vAddress;          // starting address of the region in virtual memory
    unsigned int pageSize;          // size of a (virtual) page in this PT
    unsigned int numPages;          // number of pages in this region
    unsigned int AP;                // region access permissions
    unsigned int CB;                // cache and write buffer attributes for the region
    unsigned int pAddress;          // starting address of the region in physical memory. (in the guide it says virtual but I think it's physical)
    volatile page_table_t *PT;               // is a pointer to the page table in which the region resides.
} region_t;


#endif /* KERNEL_MEMORYMANAGEMENT_MM_TYPE_H_ */
