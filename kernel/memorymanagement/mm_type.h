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
        unsigned int IGN    :30;     //Ignored [31:2]
        unsigned int TYPE   :2;      //type [1:0]
    }fld_split;
    unsigned int fdl_raw;
}fld_fault_t;

typedef union{
    struct{
        unsigned int SBA    :12;      // section base address [31:20]
        unsigned int NS     :1;       // non-secure [19]
        unsigned int SBZ    :1;       // should be zero [18]
        unsigned int nG     :1;       // not global [17]
        unsigned int S      :1;       // shared [16]
        unsigned int AP2    :1;       // access permission [15]
        unsigned int TEX    :3;       // memory region attributes bit[14:12]
        unsigned int AP1_0  :2;       // access permission [11:10]
        unsigned int IMP    :1;       // implementation defined [9]
        unsigned int DOM    :4;       // domain [8:5]
        unsigned int XN     :1;       // execute-never[4]
        unsigned int C      :1;       // cachable [3]
        unsigned int B      :1;       // bufferable[2]
        unsigned int TYPE   :2;       // type [1:0]
    }fld_split;
    unsigned int fdl_raw;
}fld_section_t;

typedef union{
    struct{
        unsigned int CPT    :22;      // coarse page table base address [31:10]
        unsigned int IMP    :1;       // implementation defined [9]
        unsigned int DOMAIN :4;       // domain [8:5]
        unsigned int SBZ    :1;       // should be zero [4]
        unsigned int NS     :1;       // non-secure [3]
        unsigned int PXN    :1;       // priveleged execute never [2]
        unsigned int TYPE   :2;       // type [1:0]
    }fld_split;
    unsigned int fdl_raw;
}fld_coarse_t;

typedef union{
    struct{
       unsigned int SPBA    :20;      // small page base address [31:12]
       unsigned int nG      :1;       // not global [11]
       unsigned int S       :1;       // shared [10]
       unsigned int AP2     :1;       // access permission [9]
       unsigned int TEX     :3;       // memory region attributes bit [8:6]
       unsigned int AP1_0   :2;       // access permission [5:4]
       unsigned int C       :1;       // cachable [3]
       unsigned int B       :1;       // bufferable [2]
       unsigned int TYPE    :2;       // type [1:0]
    }sld_split;
    unsigned int sdl_raw;
}sld_small_page_t;
#endif /* KERNEL_MEMORYMANAGEMENT_MM_TYPE_H_ */
