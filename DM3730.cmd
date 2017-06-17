MEMORY /* ARM memory map */
{
    SRAM:           o = 0x40200000  l = 0x0000FFC0  /* 64kB - 56B Internal SRAM */
    INTVECS:        o = 0x4020FFC0  l = 0x00000040  /* 56B vector table */
    //CS0_SDRAM:      o = 0x80000000  l = 0x20000000  /* 512MB of external mDDR in CS0 */
    //CS1_SDRAM:      o = 0xA0000000  l = 0x20000000  /* 512MB of external mDDR in CS1 */

    RAM0_OS:		o = 0x80000000	l = 0x80000		// 512 kB for Operating system
    RAM0_SHARED:	o = 0x80080000	l = 0x10000		// 64 kB for shared memory
    RAM0_ROOT_PT:	o = 0x80090000	l = 0x4000		// 16 kB for root PT
    RAM0_SYSTEM_PT:	o = 0x80094000	l = 0x400		// 1 kB for system PT
    RAM0_TASK_PTs:	o = 0x80094400	l = 0x4000		// 16 kB for 16 tasks (1 kb for each task)
    //space for more PTs up to 0x80493FFF
    RAM0_TASK_0:	o = 0x80494000	l = 0x100000	// 1 MB for process 0
	RAM0_TASK_1:	o = 0x80594000	l = 0x100000	// 1 MB for process 1
	RAM0_TASK_2:	o = 0x80694000	l = 0x100000	// 1 MB for process 2
	RAM0_TASK_3:	o = 0x80794000	l = 0x100000	// 1 MB for process 3
	//keep going up until 16 tasks
}

SECTIONS
{

DSP_CORE   /* ARM memory map */
	.intvecs	   >  INTVECS
	.isr		   >  SRAM
    .text          >  RAM0_OS
    .stack         >  RAM0_OS
    .bss           >  RAM0_OS
    .cio           >  RAM0_OS
    .const         >  RAM0_OS
    .data          >  RAM0_OS
    .switch        >  RAM0_OS
    .sysmem        >  RAM0_OS
    .far           >  RAM0_OS
    .args          >  RAM0_OS
    .ppinfo        >  RAM0_OS
    .ppdata        >  RAM0_OS
  
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
