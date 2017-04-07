/****************************************************************************/
/*  DM3730.cmd                                                              */
/*  Copyright (c) 2011  Texas Instruments Incorporated                      */
/*  Author: Rafael de Souza                                                 */
/*                                                                          */
/*    Description: This file is a sample linker command file that can be    */
/*                 used for linking programs built with the C compiler and  */
/*                 running the resulting .out file on a DM3730.             */
/*                 Use it as a guideline.  You will want to                 */
/*                 change the memory layout to match your specific          */
/*                 target system.  You may want to change the allocation    */
/*                 scheme according to the size of your program.            */
/*                                                                          */
/****************************************************************************/

MEMORY
{
#ifndef DSP_CORE  /* ARM memory map */

    SRAM:           o = 0x40200000  l = 0x0000FFC8  /* 64kB - 56B Internal SRAM */
    INTVECS:        o = 0x4020FFC8  l = 0x00000038  /* 56B vector table */
    CS0_SDRAM:      o = 0x80000000  l = 0x20000000  /* 512MB of external mDDR in CS0 */
    CS1_SDRAM:      o = 0xA0000000  l = 0x20000000  /* 512MB of external mDDR in CS1 */

    IVASHL2RAM:     o = 0x5C7F8000  l = 0x00008000  /* 32kB Shared IVA L2 RAM */
    IVASHL2RAM_C:   o = 0x5C800000  l = 0x00010000  /* 64kB Shared IVA L2 Cache RAM */
    IVASHL1PRAM:    o = 0x5CE00000  l = 0x00008000  /* 32kB Shared IVA L1 Program RAM */
    IVASHL1DRAM:    o = 0x5CF04000  l = 0x0000C000  /* 48kB Shared IVA L1 Data RAM */
    IVASHL1DRAM_C:  o = 0x5CF10000  l = 0x00008000  /* 32kB Shared IVA L1 Data Cache RAM */

#else             /* DSP memory map */

    IVASHL2RAM:     o = 0x007F8000  l = 0x00008000  /* 32kB IVA L2 RAM */
    IVASHL2RAM_C:   o = 0x00800000  l = 0x00010000  /* 64kB IVA L2 Cache RAM */
    IVASHL1PRAM:    o = 0x00E00000  l = 0x00008000  /* 32kB IVA L1 Program RAM */
    IVASHL1DRAM:    o = 0x00F04000  l = 0x0000C000  /* 48kB IVA L1 Data RAM */
    IVASHL1DRAM_C:  o = 0x00F10000  l = 0x00008000  /* 32kB IVA L1 Data Cache RAM */

#endif
}

SECTIONS
{
#ifndef DSP_CORE   /* ARM memory map */

	.intvecs       >  INTVECS
    .text          >  SRAM
    .stack         >  CS0_SDRAM
    .bss           >  CS0_SDRAM
    .cio           >  CS0_SDRAM
    .const         >  CS0_SDRAM
    .data          >  CS0_SDRAM
    .switch        >  CS0_SDRAM
    .sysmem        >  CS0_SDRAM
    .far           >  CS0_SDRAM
    .args          >  CS0_SDRAM
    .ppinfo        >  CS0_SDRAM
    .ppdata        >  CS0_SDRAM
  
    /* TI-ABI or COFF sections */
    .pinit         >  SRAM
    .cinit         >  SRAM
  
    /* EABI sections */
    .binit         >  SRAM
    .init_array    >  SRAM
    .neardata      >  SRAM
    .fardata       >  SRAM
    .rodata        >  SRAM
    .c6xabi.exidx  >  SRAM
    .c6xabi.extab  >  SRAM

#else              /* DSP memory map */

    .text          >  IVASHL2RAM
    .stack         >  IVASHL2RAM
    .bss           >  IVASHL2RAM
    .cio           >  IVASHL2RAM
    .const         >  IVASHL2RAM
    .data          >  IVASHL2RAM
    .switch        >  IVASHL2RAM
    .sysmem        >  IVASHL2RAM
    .far           >  IVASHL2RAM
    .args          >  IVASHL2RAM
    .ppinfo        >  IVASHL2RAM
    .ppdata        >  IVASHL2RAM
  
    /* TI-ABI or COFF sections */
    .pinit         >  IVASHL2RAM
    .cinit         >  IVASHL2RAM
  
    /* EABI sections */
    .binit         >  IVASHL2RAM
    .init_array    >  IVASHL2RAM
    .neardata      >  IVASHL2RAM
    .fardata       >  IVASHL2RAM
    .rodata        >  IVASHL2RAM
    .c6xabi.exidx  >  IVASHL2RAM
    .c6xabi.extab  >  IVASHL2RAM

#endif
}
