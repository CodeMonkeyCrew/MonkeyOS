	.global set_mmu_config_register_and_enable_mmu
	.global set_domain
	.global set_root_pt_register
	.global mmu_enable_translation_table_walk_ttbr0
	.global set_intvecs_base_address
	.global mmu_flush_tlb
	.global mmu_flush_cache

;-------------------------------------------------------------------------------------------
; 				Enable MMU, Data Cache and Instruction Cache							   -
;-------------------------------------------------------------------------------------------
set_mmu_config_register_and_enable_mmu:
	AND		r0,r0,#0					;clear register
	ORR 	r0,r0,#0x1					;enable mmu
	ORR		r0,r0,#0x2					;enable alignment checking
	;ORR 	r0,r0,#0x3					;enable datacache
	;ORR 	r0,r0,#0x1000				;enable instruction cache
	MCR 	p15,#0x0,r0,c1,c0,#0		;read r0 register and write into config register
	MOV		PC, LR						;jump back to calling func

set_domain:
	AND 	r0,r0,#0					;clear the register -> removes all access permissions of all domains
	ORR 	r0,r0,#0x40					;set to access permission for domain 3 (as we only use domain 3)
	MCR 	p15,#0x0,r0,c3,c0,#0		;write r0 register to domain config register
	MOV		PC, LR						;jump back to calling func


set_root_pt_register:
	BIC		r0, r0, r1					;clear bits [13:0] to create the proper Translation Table Base
	MCR 	p15,#0x0,r0,c2,c0,#0		;write root pt address into Translation Table Base Register
	MOV		PC, LR						;jump back to calling func

mmu_enable_translation_table_walk_ttbr0:
	MRC 	p15,#0x0,r0,c2,c0,#2		;read register into r1
	BIC		r0, r0, #0x8				;only clear bit [4] -> P0 which enables the table walk through of the TTBR0
	MCR		p15,#0x0,r0,c2,c0,#2		;write to enable
	MOV		PC, LR

set_intvecs_base_address:
	MCR 	p15,#0x0,r0,c12,c0,#0		;write intvecs base address to the address
	MOV		PC, LR						;jump back to calling func

mmu_flush_cache:
	MOV		r0,#0
	MCR		p15,#0x0,r0,c7,c5,#0		;flush instruction cache
	MCR		p15,#0x0,r0,c7,c6,#2		;flush data cache
	MOV		PC, LR						;jump back to calling func


mmu_flush_tlb:
	MOV		r0,#0
	MCR		p15,#0x0,r0,c8,c5,#0		;invalidates entire instruction TLB		maybe comment out..
	MCR		p15,#0x0,r0,c8,c6,#0		;invalidates entire data TLB
	MOV		PC, LR						;jump back to calling func
