	.global mode_setUserMode

mode_setUserMode:
	MRS R0, CPSR
	BIC R0, R0, #0xC0	; enable IRQ / FIQ
	BIC R0, R0, #0x1F	; clear modes
	ORR R0, R0, #0x10	; set user mode
	MSR CPSR_cf, R0
	MOV PC, LR
