	.global dispatcher_switchContext
	.global dispatcher_saveContextFork
	.global dispatcher_saveContextWaitPid
	.global dispatcher_loadContext
	.global dispatcher_storeContext



dispatcher_switchContext:
	; TODO: check if there are any options other than manipulating the stack pointer
	ADD R13, R13, #8				; stack pointer correction
	; first store the old process�s User mode state to the PCB pointed to by R0
	MRS R12, SPSR					; get CPSR of interrupted process
	STR R12, [R0], #8				; store CPSR to PCB, point R0 at PCB location for R0 value
	LDMFD R13!, {R2, R3}			; reload R0/R1 of interrupted process from stack
	STMIA R0!, {R2, R3}				; store R0/R1 values to PCB, point R0 at PCB location for R2 value
	LDMFD R13!, {R2-R12, R14}		; reload remaining stacked values
	STR R14, [R0, #-12]				; store R14_irq, the interrupted process�s restart address
	STMIA R0, {R2-R14}^				; store user R2-R14
	; then load the new process User mode state and return to it
	LDMIA R1!, {R12, R14}
	MSR SPSR_fsxc, R12
	LDMIA R1, {R0-R14}^
	NOP
	MOVS PC, LR

dispatcher_storeContext:
	; TODO: check if there are any options other than manipulating the stack pointer
	; TODO CHANGE STACKPOINTER
	ADD R13, R13, #8				; stack pointer correction
	; first store the old process�s User mode state to the PCB pointed to by R0
	MRS R12, SPSR					; get CPSR of interrupted process
	STR R12, [R0], #8				; store CPSR to PCB, point R0 at PCB location for R0 value
	LDMFD R13!, {R2, R3}			; reload R0/R1 of interrupted process from stack
	STMIA R0!, {R2, R3}				; store R0/R1 values to PCB, point R0 at PCB location for R2 value
	LDMFD R13!, {R2-R12, R14}		; reload remaining stacked values
	STR R14, [R0, #-12]				; store R14_irq, the interrupted process�s restart address
	STMIA R0, {R2-R14}^				; store user R2-R14
	MOVS PC, LR

dispatcher_loadContext:
	LDMIA R0!, {R12, R14}
	MSR SPSR_fsxc, R12
	LDMIA R0, {R0-R14}^
	NOP
	MOVS PC, LR


dispatcher_saveContextFork:
	; TODO: check if there are any options other than manipulating the stack pointer
	ADD R1, R13, #0x18				; stack pointer correction
	STMFD SP!, {R4-R12, R14}		; push the Registers R4-R12 and R14 to the Stack
	; first store the old process�s User mode state to the PCB pointed to by R0
	MRS R12, SPSR					; get CPSR of interrupted process
	STR R12, [R0], #8				; store CPSR to PCB, point R0 at PCB location for R0 value
	LDMFD R1!, {R2, R3}				; reload R0/R1 of interrupted process from stack
	STMIA R0!, {R2, R3}				; store R0/R1 values to PCB, point R0 at PCB location for R2 value
	LDMFD R1!, {R2-R12, R14}		; reload remaining stacked values
	STR R14, [R0, #-12]				; store R14_irq, the interrupted process�s restart address
	STMIA R0, {R2-R14}^				; store user R2-R14
	LDMFD SP!, {R4-R12, R14}
	MOV PC, R14

dispatcher_saveContextWaitPid:
	; TODO: check if there are any options other than manipulating the stack pointer
	ADD R1, R13, #0x10				; stack pointer correction
	STMFD SP!, {R4-R12, R14}		; push the Registers R4-R12 and R14 to the Stack
	; first store the old process�s User mode state to the PCB pointed to by R0
	MRS R12, SPSR					; get CPSR of interrupted process
	STR R12, [R0], #8				; store CPSR to PCB, point R0 at PCB location for R0 value
	LDMFD R1!, {R2, R3}				; reload R0/R1 of interrupted process from stack
	STMIA R0!, {R2, R3}				; store R0/R1 values to PCB, point R0 at PCB location for R2 value
	LDMFD R1!, {R2-R12, R14}		; reload remaining stacked values
	STR R14, [R0, #-12]				; store R14_irq, the interrupted process�s restart address
	STMIA R0, {R2-R14}^				; store user R2-R14
	LDMFD SP!, {R4-R12, R14}
	MOV PC, R14


mmu_flush_cache:
	MOV		r0,#0
	MCR		p15,#0x0,r0,c7,c5,#0		;flush instruction cache
	MCR		p15,#0x0,r0,c7,c6,#0		;flush data cache
	MOV		PC, LR


mmu_flush_tlb:
	MOV		r0,#0
	MCR		p15,#0x0,r0,c8,c5,#0		;invalidates entire instruction TLB
	MCR		p15,#0x0,r0,c8,c6,#0		;invalidates entire data TLB
	MOV		PC, LR
