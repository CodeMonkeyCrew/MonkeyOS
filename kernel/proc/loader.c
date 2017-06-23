#include "loader.h"
#include "proc.h"
#include "../memorymanagement/mmu.h"
#include <stdio.h>
#include <string.h>

static int checkElf(const Elf32_Ehdr* elfHeader)
{
    if (elfHeader == NULL)
    {
        return -1;
    }
    if (elfHeader->e_ident[EI_MAG0] != ELFMAG0)
    {
        printf("ELF Header EI_MAG0 incorrect\n");
        return -1;
    }
    if (elfHeader->e_ident[EI_MAG1] != ELFMAG1)
    {
        printf("ELF Header EI_MAG1 incorrect\n");
        return -1;
    }
    if (elfHeader->e_ident[EI_MAG2] != ELFMAG2)
    {
        printf("ELF Header EI_MAG2 incorrect\n");
        return -1;
    }
    if (elfHeader->e_ident[EI_MAG3] != ELFMAG3)
    {
        printf("ELF Header EI_MAG3 incorrect\n");
        return -1;
    }
    return 0;
}

static uint8_t* getSectionHeaderTable(const Elf32_Ehdr* elfHeader)
{
    return ((uint8_t*) elfHeader) + elfHeader->e_shoff;
}

static Elf32_Shdr* getSectionHeader(const Elf32_Ehdr* elfHeader, int index)
{
    return (Elf32_Shdr*) (getSectionHeaderTable(elfHeader)
            + elfHeader->e_shentsize * index);
}

static uint8_t* getProgramHeaderTable(const Elf32_Ehdr* elfHeader)
{
    return ((uint8_t*) elfHeader) + elfHeader->e_phoff;
}

static Elf32_Phdr* getProgramHeader(const Elf32_Ehdr* elfHeader, int index)
{
    return (Elf32_Phdr*) (getProgramHeaderTable(elfHeader)
            + elfHeader->e_phentsize * index);
}

static Elf32_Shdr* getSectionNameStringTable(const Elf32_Ehdr* elfHeader)
{
    return getSectionHeader(elfHeader, elfHeader->e_shtrndx);
}

static char* getSectionName(const Elf32_Ehdr* elfHeader, int index)
{
    Elf32_Shdr* sectionNameStringTable = getSectionNameStringTable(elfHeader);
    return ((char*) elfHeader) + sectionNameStringTable->sh_offset + index;
}

static void printElfHeader(const Elf32_Ehdr* elfHeader)
{
    printf("ELF header\n");
    printf("------------------------------------\n");
    printf("e_ident:\n");
    printf("\tEI_MAG0: 0x%02x\n", elfHeader->e_ident[EI_MAG0]);
    printf("\tEI_MAG1: %c\n", elfHeader->e_ident[EI_MAG1]);
    printf("\tEI_MAG2: %c\n", elfHeader->e_ident[EI_MAG2]);
    printf("\tEI_MAG3: %c\n", elfHeader->e_ident[EI_MAG3]);
    printf("\tEI_CLASS: %d\n", elfHeader->e_ident[EI_CLASS]);
    printf("\tEI_DATA: %d\n", elfHeader->e_ident[EI_DATA]);
    printf("\tEI_VERSION: %d\n", elfHeader->e_ident[EI_VERSION]);
    printf("\tEI_OSABI: %d\n", elfHeader->e_ident[EI_OSABI]);
    printf("\tEI_ABIVERSION: %d\n", elfHeader->e_ident[EI_ABIVERSION]);
    printf("\tEI_PAD: %d\n", elfHeader->e_ident[EI_PAD]);
    printf("e_type: 0x%04x\n", elfHeader->e_type);
    printf("e_machine: 0x%04x\n", elfHeader->e_machine);
    printf("e_version: 0x%08x\n", elfHeader->e_version);
    printf("e_entry: 0x%08x\n", elfHeader->e_entry);
    printf("e_phoff: 0x%08x\n", elfHeader->e_phoff);
    printf("e_shoff: 0x%08x\n", elfHeader->e_shoff);
    printf("e_flags: 0x%08x\n", elfHeader->e_flags);
    printf("e_ehsize: 0x%04x\n", elfHeader->e_ehsize);
    printf("e_phentsize: 0x%04x\n", elfHeader->e_phentsize);
    printf("e_phnum: 0x%04x\n", elfHeader->e_phnum);
    printf("e_shentsize: 0x%04x\n", elfHeader->e_shentsize);
    printf("e_shnum: 0x%04x\n", elfHeader->e_shnum);
    printf("e_shtrndx: 0x%04x\n", elfHeader->e_shtrndx);
}

static void printSectionHeader(const Elf32_Ehdr* elfHeader, int index)
{
    Elf32_Shdr* sectionHeader = getSectionHeader(elfHeader, index);
    printf("sh_name: %s\n", getSectionName(elfHeader, sectionHeader->sh_name));
    printf("sh_type: 0x%08x\n", sectionHeader->sh_type);
    printf("sh_flags: 0x%08x\n", sectionHeader->sh_flags);
    printf("sh_addr: 0x%08x\n", sectionHeader->sh_addr);
    printf("sh_offset: 0x%08x\n", sectionHeader->sh_offset);
    printf("sh_size: 0x%08x\n", sectionHeader->sh_size);
    printf("sh_link: 0x%08x\n", sectionHeader->sh_link);
    printf("sh_info: 0x%08x\n", sectionHeader->sh_info);
    printf("sh_addralign: 0x%08x\n", sectionHeader->sh_addralign);
    printf("sh_entsize: 0x%08x\n", sectionHeader->sh_entsize);
}

static void printSectionHeaderTable(const Elf32_Ehdr* elfHeader)
{
    printf("ELF section header table\n");
    printf("------------------------------------\n");

    int index = 0;
    for (index = 0; index < elfHeader->e_shnum; ++index)
    {
        printSectionHeader(elfHeader, index);
    }
}

static void printProgramHeader(const Elf32_Ehdr* elfHeader, int index)
{
    Elf32_Phdr* programHeader = getProgramHeader(elfHeader, index);
    printf("p_type: 0x%08x\n", programHeader->p_type);
    printf("p_offset: 0x%08x\n", programHeader->p_offset);
    printf("p_vaddr: 0x%08x\n", programHeader->p_vaddr);
    printf("p_paddr: 0x%08x\n", programHeader->p_paddr);
    printf("p_filesz: 0x%08x\n", programHeader->p_filesz);
    printf("p_memsz: 0x%08x\n", programHeader->p_memsz);
    printf("p_flags: 0x%08x\n", programHeader->p_flags);
    printf("p_align: 0x%08x\n", programHeader->p_align);
}

static void printProgramHeaderTable(const Elf32_Ehdr* elfHeader)
{
    printf("ELF program header table\n");
    printf("------------------------------------\n");

    int index = 0;
    for (index = 0; index < elfHeader->e_phnum; ++index)
    {
        printProgramHeader(elfHeader, index);
    }
}

void printElf(const Elf32_Ehdr* elfHeader)
{
    printElfHeader(elfHeader);
    printSectionHeaderTable(elfHeader);
    printProgramHeaderTable(elfHeader);
}

int loadElf(PCB_t* proc, const Elf32_Ehdr* elfHeader)
{
    int isValid = checkElf(elfHeader);
    if (isValid >= 0)
    {
        // set pid on mmu
        mmu_load_task_region(proc->pid);

        // load data from elf file into main memory
        int sectionIndex = 0;
        for (sectionIndex = 0; sectionIndex < elfHeader->e_shnum;
                ++sectionIndex)
        {
            Elf32_Shdr* sectionHeader = getSectionHeader(elfHeader,
                                                         sectionIndex);
            if (sectionHeader->sh_size > 0)
            {
                char* sectionName = getSectionName(elfHeader,
                                                   sectionHeader->sh_name);
                uint8_t* section = ((uint8_t*) elfHeader)
                        + sectionHeader->sh_offset;

                if (strcmp(sectionName, ".text") == 0
                        || strcmp(sectionName, ".data") == 0)
                {
                    // copy text and data segments into main memory
                    memcpy((uint8_t*) sectionHeader->sh_addr, section,
                           sectionHeader->sh_size);
                }
                if (strcmp(sectionName, ".bss") == 0)
                {
                    // set non-initialized data to 0
                    uint8_t* sectionEnd = section + sectionHeader->sh_size;
                    while (section != sectionEnd)
                    {
                        *section++ = 0;
                    }
                }
                if (strcmp(sectionName, ".stack") == 0)
                {
                    // set stack pointer
                    proc->context.sp = sectionHeader->sh_addr;
                }
            }
        }
        // set restartAddress to entry point
        proc->context.restartAddress = elfHeader->e_entry;
    }
    return isValid;
}
