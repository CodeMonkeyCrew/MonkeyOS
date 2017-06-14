MEMORY /* ARM memory map */
{
    SRAM:           o = 0x40200000  l = 0x0000FFC0  /* 64kB - 56B Internal SRAM */    //decreased size by 0x8 as int
    INTVECS:        o = 0x4020FFC0  l = 0x00000040  /* 56B vector table */
    CS0_SDRAM:      o = 0x80000000  l = 0x20000000  /* 512MB of external mDDR in CS0 */
    CS1_SDRAM:      o = 0xA0000000  l = 0x20000000  /* 512MB of external mDDR in CS1 */
}

SECTIONS
{
DSP_CORE   /* ARM memory map */

	.intvecs	   >  INTVECS
    .text          >  SRAM
    .stack         >  SRAM
    .bss           >  SRAM
    .cio           >  SRAM
    .const         >  SRAM
    .data          >  SRAM
    .switch        >  SRAM
    .sysmem        >  SRAM
    .far           >  SRAM
    .args          >  SRAM
    .ppinfo        >  SRAM
    .ppdata        >  SRAM
  
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

