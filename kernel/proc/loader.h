#ifndef KERNEL_PROC_LOADER_H_
#define KERNEL_PROC_LOADER_H_

#include <inttypes.h>

#define ELF_NIDENT 16

enum Elf_Ident
{
    EI_MAG0 = 0,        // 0x7F
    EI_MAG1 = 1,        // 'E'
    EI_MAG2 = 2,        // 'L'
    EI_MAG3 = 3,        // 'F'
    EI_CLASS = 4,       // 1, 32-bit architecture
    EI_DATA = 5,        // 1, LSB byte order
    EI_VERSION = 6,     // 1, ELF version
    EI_OSABI = 7,       // 0, OS specific
    EI_ABIVERSION = 8,  // 0, OS specific
    EI_PAD = 9          // 0, padding
};

typedef uint16_t Elf32_Half;    // Unsigned half int
typedef uint32_t Elf32_Off;     // Unsigned offset
typedef uint32_t Elf32_Addr;    // Unsigned address
typedef uint32_t Elf32_Word;    // Unsigned int
typedef int32_t Elf32_Sword;    // Signed int

typedef struct
{
    uint8_t e_ident[ELF_NIDENT];
    Elf32_Half e_type;          // 0x0002, executable file
    Elf32_Half e_machine;       // 0x0028, arm cortex a8
    Elf32_Word e_version;       // 0x00000001, current version
    Elf32_Addr e_entry;         // 0x40200bcc, entry point (virtual address)
    Elf32_Off e_phoff;          // 0x00008124, program header table offset
    Elf32_Off e_shoff;          // 0x000081a4, section header table offset
    Elf32_Word e_flags;         // 0x05000000, processor specific flags
    Elf32_Half e_ehsize;        // 0x0034, elf header size
    Elf32_Half e_phentsize;     // 0x0020, program header table entry size
    Elf32_Half e_phnum;         // 0x0004, number of entries in the program header table
    Elf32_Half e_shentsize;     // 0x0028, section header table entry size
    Elf32_Half e_shnum;         // 0x0025, number of entries in the section header table
    Elf32_Half e_shtrndx;       // 0x0024, section header table index of section name string table
} Elf32_Ehdr;

typedef struct
{
    Elf32_Word sh_name;         // index of the section name in the string table
    Elf32_Word sh_type;         // categorizes the section's contents and semantics
    Elf32_Word sh_flags;        // 1-bit flags that describe miscellaneous attributes
    Elf32_Addr sh_addr;         // section address in the memory image of a process
    Elf32_Off sh_offset;        // section offset
    Elf32_Word sh_size;         // section size
    Elf32_Word sh_link;         // section header table index link (interpretation depends on the section type)
    Elf32_Word sh_info;         // extra information, (interpretation depends on the section type)
    Elf32_Word sh_addralign;    // address alignment constraints
    Elf32_Word sh_entsize;      // table entry size
} Elf32_Shdr;

typedef struct
{
    Elf32_Word p_type;          // kind of segment
    Elf32_Off p_offset;         // segment offset
    Elf32_Addr p_vaddr;         // virtual address of the segment
    Elf32_Addr p_paddr;         // physical address of the segment
    Elf32_Word p_filesz;        // segment size in the file image
    Elf32_Word p_memsz;         // segment size in the memory image
    Elf32_Word p_flags;         // flags relevant to the segment
    Elf32_Word p_align;         // value to which the segments are aligned in memory and in the file
} Elf32_Phdr;

void printElf(const Elf32_Ehdr* elfHeader);

#endif /* KERNEL_PROC_LOADER_H_ */
