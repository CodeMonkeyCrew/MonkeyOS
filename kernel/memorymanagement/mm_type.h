#ifndef KERNEL_MEMORYMANAGEMENT_MM_TYPE_H_
#define KERNEL_MEMORYMANAGEMENT_MM_TYPE_H_

/*
 * see arm system developers guide, chapter 14
 * NOTE: Root Page Table = Master Page Table
 */

typedef enum {FAULT, COARSE, ROOT} page_table_type_t;

typedef struct {
    unsigned int vAddress;          // identifies the starting address of a 1 MB section of virtual memory controlled by either a section entry or an L2 page table.
    unsigned int ptAddress;         // is the address where the page table is located in virtual memory
    unsigned int rootPTAddress;     // is the address of the master L1 (root) page table. If it is the root PT the value is the same as ptAddress
    page_table_type_t type;           // type of the PT
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

typedef union{
    struct{
        unsigned int IGN : 30;      //Ignored [31:2]
        unsigned int TYPE : 2;       //type [1:0]
    }fdl_split;
    unsigned int fdl_raw;
}fld_faul_t;
typedef union{
    struct{
        unsigned int SBA : 12;      //section base address [31:20]
        unsigned int SBZ1 : 8;       //should be zero [19:12]
        unsigned int AP : 2;        //access persmission [11:10]
        unsigned int SBZ2 : 1;       //should be zero [9]
        unsigned int DOMAIN: 4;     //domain [8:5]
        unsigned int IMP : 3;       //implementation defined [4:2]
        unsigned int TYPE :2;         //type [1:0]
    }fdl_split;
    unsigned int fdl_raw;
}fld_section_t;

typedef union{
    struct{
        unsigned int CPT:22;        //coarse page table base address [31:10]
        unsigned int SBZ:1;         //should be zero [9]
        unsigned int DOMAIN:4;      //domain [8:5]
        unsigned int IMP:3;         //implementation defined [4:2]
        unsigned int TYPE:2;         //type [1:0]
    }fdl_split;
}fld_coarse_t;

#endif /* KERNEL_MEMORYMANAGEMENT_MM_TYPE_H_ */
