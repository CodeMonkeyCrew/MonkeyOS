	.global dispatcher_switchContext

dispatcher_switchContext:
	; TODO: check if there are any options other than manipulating the stack pointer
	ADD R13, R13, #8				; stack pointer correction
	; first store the old process´s User mode state to the PCB pointed to by R0
	MRS R12, SPSR					; get CPSR of interrupted process
	STR R12, [R0], #8				; store CPSR to PCB, point R0 at PCB location for R0 value
	LDMFD R13!, {R2, R3}			; reload R0/R1 of interrupted process from stack
	STMIA R0!, {R2, R3}				; store R0/R1 values to PCB, point R0 at PCB location for R2 value
	LDMFD R13!, {R2-R12, R14}		; reload remaining stacked values
	STR R14, [R0, #-12]				; store R14_irq, the interrupted process´s restart address
	STMIA R0, {R2-R14}^				; store user R2-R14
	; then load the new process´s User mode state and return to it
	LDMIA R1!, {R12, R14}
	MSR SPSR_fsxc, R12
	LDMIA R1, {R0-R14}^
	NOP
	MOVS PC, R14
