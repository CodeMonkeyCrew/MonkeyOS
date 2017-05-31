	.global dispatcher_switchContext
	.global dispatcher_saveContext

dispatcher_switchContext:
	; TODO: check if there are any options other than manipulating the stack pointer
	ADD R13, R13, #8				; stack pointer correction
	; first store the old process큦 User mode state to the PCB pointed to by R0
	MRS R12, SPSR					; get CPSR of interrupted process
	STR R12, [R0], #8				; store CPSR to PCB, point R0 at PCB location for R0 value
	LDMFD R13!, {R2, R3}			; reload R0/R1 of interrupted process from stack
	STMIA R0!, {R2, R3}				; store R0/R1 values to PCB, point R0 at PCB location for R2 value
	LDMFD R13!, {R2-R12, R14}		; reload remaining stacked values
	STR R14, [R0, #-12]				; store R14_irq, the interrupted process큦 restart address
	STMIA R0, {R2-R14}^				; store user R2-R14
	; then load the new process큦 User mode state and return to it
	LDMIA R1!, {R12, R14}			; !!! R14 is getting 0 !!!
	MSR SPSR_fsxc, R12
	LDMIA R1, {R0-R14}^
	NOP
	MOVS PC, R14

dispatcher_saveContext:
	; TODO: check if there are any options other than manipulating the stack pointer
	ADD R1, R13, #0x08				; stack pointer correction
	STMFD SP!, {R4-R12, R14}		; push the Registers R4-R12 and R14 to the Stack
	; first store the old process큦 User mode state to the PCB pointed to by R0
	MRS R12, SPSR					; get CPSR of interrupted process
	STR R12, [R0], #8				; store CPSR to PCB, point R0 at PCB location for R0 value
	LDMFD R1!, {R2, R3}				; reload R0/R1 of interrupted process from stack
	STMIA R0!, {R2, R3}				; store R0/R1 values to PCB, point R0 at PCB location for R2 value
	LDMFD R1!, {R2-R12, R14}		; reload remaining stacked values
	STR R14, [R0, #-12]				; store R14_irq, the interrupted process큦 restart address
	STMIA R0, {R2-R14}^				; store user R2-R14
	LDMFD SP!, {R4-R12, R14}
	MOV PC, R14

