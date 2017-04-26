MEMORY /* ARM memory map */
{
    SRAM:           o = 0x40200000  l = 0x0000FFC8  /* 64kB - 56B Internal SRAM */
    INTVECS:        o = 0x4020FFC8  l = 0x00000038  /* 56B vector table */
    CS0_SDRAM:      o = 0x80000000  l = 0x20000000  /* 512MB of external mDDR in CS0 */
    CS1_SDRAM:      o = 0xA0000000  l = 0x20000000  /* 512MB of external mDDR in CS1 */

    IVASHL2RAM:     o = 0x5C7F8000  l = 0x00008000  /* 32kB Shared IVA L2 RAM */
    IVASHL2RAM_C:   o = 0x5C800000  l = 0x00010000  /* 64kB Shared IVA L2 Cache RAM */
    IVASHL1PRAM:    o = 0x5CE00000  l = 0x00008000  /* 32kB Shared IVA L1 Program RAM */
    IVASHL1DRAM:    o = 0x5CF04000  l = 0x0000C000  /* 48kB Shared IVA L1 Data RAM */
    IVASHL1DRAM_C:  o = 0x5CF10000  l = 0x00008000  /* 32kB Shared IVA L1 Data Cache RAM */
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
}
