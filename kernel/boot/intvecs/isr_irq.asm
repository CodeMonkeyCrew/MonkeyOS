	   	.if __TI_EABI_ASSEMBLER
		.asg scheduler_run, C_SCHEDULER_RUN
	    .else
	    .asg _scheduler_run, C_SCHEDULER_RUN
	    .endif

	.global _ISR_IRQ
	.global C_SCHEDULER_RUN

; INTCPS_SIR_IRQ register address
INTCPS_SIR_IRQ_ADDR .word 0x48200040
; ACTIVEIRQ bit field mask to get only the bit field
ACTIVEIRQ_MASK .equ 0x7F

_ISR_IRQ:
	; calculate return address
	SUB LR, LR, #4
	; Save the critical context
	STMFD SP!, {R0-R12, LR}				; Save working registers and the Link register
	MRS R11, SPSR						; Save the SPSR into R11

	; Get the number of the highest priority active IRQ
	LDR R10, INTCPS_SIR_IRQ_ADDR
	LDR R10, [R10] 						; Get the INTCPS_SIR_IRQ register
	AND R10, R10, #ACTIVEIRQ_MASK		; Apply the mask to get the active IRQ number

	ADR LR, ISR_IRQ_end					; Set LR to return to ISR_IRQ_end after subroutine handler

    ; Jump to relevant subroutine handler
    LDR PC, [PC, R10, lsl #2]			; PC base address points this instruction + 8
    NOP                             	; To index the table by the PC

	; Table of handler start addresses
	.word IRQDefaultHandler				; for IRQ0
	.word IRQDefaultHandler				; for IRQ1
	.word IRQDefaultHandler				; for IRQ2
	.word IRQDefaultHandler				; for IRQ3
	.word IRQDefaultHandler				; for IRQ4
	.word IRQDefaultHandler				; for IRQ5
	.word IRQDefaultHandler				; for IRQ6
	.word IRQDefaultHandler				; for IRQ7
	.word IRQDefaultHandler				; for IRQ8
	.word IRQDefaultHandler				; for IRQ9
	.word IRQDefaultHandler				; for IRQ10
	.word IRQDefaultHandler				; for IRQ11
	.word IRQDefaultHandler				; for IRQ12
	.word IRQDefaultHandler				; for IRQ13
	.word IRQDefaultHandler				; for IRQ14
	.word IRQDefaultHandler				; for IRQ15
	.word IRQDefaultHandler				; for IRQ16
	.word IRQDefaultHandler				; for IRQ17
	.word IRQDefaultHandler				; for IRQ18
	.word IRQDefaultHandler				; for IRQ19
	.word IRQDefaultHandler				; for IRQ20
	.word IRQDefaultHandler				; for IRQ21
	.word IRQDefaultHandler				; for IRQ22
	.word IRQDefaultHandler				; for IRQ23
	.word IRQDefaultHandler				; for IRQ24
	.word IRQDefaultHandler				; for IRQ25
	.word IRQDefaultHandler				; for IRQ26
	.word IRQDefaultHandler				; for IRQ27
	.word IRQDefaultHandler				; for IRQ28
	.word IRQDefaultHandler				; for IRQ29
	.word IRQDefaultHandler				; for IRQ30
	.word IRQDefaultHandler				; for IRQ31
	.word IRQDefaultHandler				; for IRQ32
	.word IRQDefaultHandler				; for IRQ33
	.word IRQDefaultHandler				; for IRQ34
	.word IRQDefaultHandler				; for IRQ35
	.word IRQDefaultHandler				; for IRQ36
	.word IRQDefaultHandler				; for IRQ37
	.word C_SCHEDULER_RUN				; for IRQ38
	.word IRQDefaultHandler				; for IRQ39
	.word IRQDefaultHandler				; for IRQ40
	.word IRQDefaultHandler				; for IRQ41
	.word IRQDefaultHandler				; for IRQ42
	.word IRQDefaultHandler				; for IRQ43
	.word IRQDefaultHandler				; for IRQ44
	.word IRQDefaultHandler				; for IRQ45
	.word IRQDefaultHandler				; for IRQ46
	.word IRQDefaultHandler				; for IRQ47
	.word IRQDefaultHandler				; for IRQ48
	.word IRQDefaultHandler				; for IRQ49
	.word IRQDefaultHandler				; for IRQ50
	.word IRQDefaultHandler				; for IRQ51
	.word IRQDefaultHandler				; for IRQ52
	.word IRQDefaultHandler				; for IRQ53
	.word IRQDefaultHandler				; for IRQ54
	.word IRQDefaultHandler				; for IRQ55
	.word IRQDefaultHandler				; for IRQ56
	.word IRQDefaultHandler				; for IRQ57
	.word IRQDefaultHandler				; for IRQ58
	.word IRQDefaultHandler				; for IRQ59
	.word IRQDefaultHandler				; for IRQ60
	.word IRQDefaultHandler				; for IRQ61
	.word IRQDefaultHandler				; for IRQ62
	.word IRQDefaultHandler				; for IRQ63
	.word IRQDefaultHandler				; for IRQ64
	.word IRQDefaultHandler				; for IRQ65
	.word IRQDefaultHandler				; for IRQ66
	.word IRQDefaultHandler				; for IRQ67
	.word IRQDefaultHandler				; for IRQ68
	.word IRQDefaultHandler				; for IRQ69
	.word IRQDefaultHandler				; for IRQ70
	.word IRQDefaultHandler				; for IRQ71
	.word IRQDefaultHandler				; for IRQ72
	.word IRQDefaultHandler				; for IRQ73
	.word IRQDefaultHandler				; for IRQ74
	.word IRQDefaultHandler				; for IRQ75
	.word IRQDefaultHandler				; for IRQ76
	.word IRQDefaultHandler				; for IRQ77
	.word IRQDefaultHandler				; for IRQ78
	.word IRQDefaultHandler				; for IRQ79
	.word IRQDefaultHandler				; for IRQ80
	.word IRQDefaultHandler				; for IRQ81
	.word IRQDefaultHandler				; for IRQ82
	.word IRQDefaultHandler				; for IRQ83
	.word IRQDefaultHandler				; for IRQ84
	.word IRQDefaultHandler				; for IRQ85
	.word IRQDefaultHandler				; for IRQ86
	.word IRQDefaultHandler				; for IRQ87
	.word IRQDefaultHandler				; for IRQ88
	.word IRQDefaultHandler				; for IRQ89
	.word IRQDefaultHandler				; for IRQ90
	.word IRQDefaultHandler				; for IRQ91
	.word IRQDefaultHandler				; for IRQ92
	.word IRQDefaultHandler				; for IRQ93
	.word IRQDefaultHandler				; for IRQ94
	.word IRQDefaultHandler				; for IRQ95

IRQDefaultHandler:
	B IRQDefaultHandler

GPTIMER2:
	B GPTIMER2

; INTCPS_CONTROL register address
INTCPS_CONTROL_ADDR .word 0x48200048;
; NEWIRQAGR bit mask to set only the NEWIRQAGR bit
NEWIRQAGR_MASK .equ 0x01

ISR_IRQ_end:
	; Allow new IRQs at INTC side
	; The INTCPS_CONTROL register is a write only register so no need to write back others bits

	MOV R0, #NEWIRQAGR_MASK				; Get the NEWIRQAGR bit position LDR R1, INTCPS_CONTROL_ADDR
	LDR R1, INTCPS_CONTROL_ADDR
	STR R0, [R1]						; Write the NEWIRQAGR bit to allow new IRQs

    ; Data Synchronization Barrier
	MOV R0, #0
    MCR P15, #0, R0, C7, C10, #4

    ; restore critical context
    MSR SPSR_cf, R11					; Restore the SPSR from R11
    LDMFD SP!, {R0-R12, LR}				; Restore working registers and Link register

    ; Return after handling the interrupt
    MOVS PC, LR
