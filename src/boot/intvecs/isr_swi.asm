	   	.if __TI_EABI_ASSEMBLER
		.asg openISR, C_OPEN_ISR
		.asg closeISR, C_CLOSE_ISR
		.asg writeISR, C_WRITE_ISR
	    .else
	    .asg _openISR, C_PUT_ISR
	    .asg _closeISR, C_CLOSE_ISR
	    .endif

	.global _ISR_SWI
	.global C_OPEN_ISR
	.global C_CLOSE_ISR
	.global C_WRITE_ISR

; ACTIVESWI bit field mask to get only the bit field
ACTIVESWI_MASK .equ 0x3F

_ISR_SWI:
	; Save the critical context
	STMFD SP!, {R1-R12, LR}				; Save working registers and the Link register
	MRS R11, SPSR						; Save the SPSR into R11

	; Get the number of SWI
	SUB R10, LR, #4						; Get previous instruction (LR - 4)
	LDR R10, [R10] 						; Get the value of the previous SWI instruction
	AND R10, R10, #ACTIVESWI_MASK		; Apply the mask to get the active SWI number

	ADR LR, ISR_SWI_end					; Set LR to return to ISR_SWI_end after subroutine handler

    ; Jump to relevant subroutine handler
    LDR PC, [PC, R10, lsl #2]			; PC base address points this instruction + 8
    NOP                             	; To index the table by the PC

	; Table of handler start addresses
	.word C_OPEN_ISR					; for SWI0
	.word C_CLOSE_ISR					; for SWI1
	.word C_WRITE_ISR					; for SWI2
	.word SWIDefaultHandler				; for SWI3
	.word SWIDefaultHandler				; for SWI4
	.word SWIDefaultHandler				; for SWI5
	.word SWIDefaultHandler				; for SWI6
	.word SWIDefaultHandler				; for SWI7
	.word SWIDefaultHandler				; for SWI8
	.word SWIDefaultHandler				; for SWI9
	.word SWIDefaultHandler				; for SWI10
	.word SWIDefaultHandler				; for SWI11
	.word SWIDefaultHandler				; for SWI12
	.word SWIDefaultHandler				; for SWI13
	.word SWIDefaultHandler				; for SWI14
	.word SWIDefaultHandler				; for SWI15
	.word SWIDefaultHandler				; for SWI16
	.word SWIDefaultHandler				; for SWI17
	.word SWIDefaultHandler				; for SWI18
	.word SWIDefaultHandler				; for SWI19
	.word SWIDefaultHandler				; for SWI20
	.word SWIDefaultHandler				; for SWI21
	.word SWIDefaultHandler				; for SWI22
	.word SWIDefaultHandler				; for SWI23
	.word SWIDefaultHandler				; for SWI24
	.word SWIDefaultHandler				; for SWI25
	.word SWIDefaultHandler				; for SWI26
	.word SWIDefaultHandler				; for SWI27
	.word SWIDefaultHandler				; for SWI28
	.word SWIDefaultHandler				; for SWI29
	.word SWIDefaultHandler				; for SWI30
	.word SWIDefaultHandler				; for SWI31
	.word SWIDefaultHandler				; for SWI32
	.word SWIDefaultHandler				; for SWI33
	.word SWIDefaultHandler				; for SWI34
	.word SWIDefaultHandler				; for SWI35
	.word SWIDefaultHandler				; for SWI36
	.word SWIDefaultHandler				; for SWI37
	.word SWIDefaultHandler				; for SWI38
	.word SWIDefaultHandler				; for SWI39
	.word SWIDefaultHandler				; for SWI40
	.word SWIDefaultHandler				; for SWI41
	.word SWIDefaultHandler				; for SWI42
	.word SWIDefaultHandler				; for SWI43
	.word SWIDefaultHandler				; for SWI44
	.word SWIDefaultHandler				; for SWI45
	.word SWIDefaultHandler				; for SWI46
	.word SWIDefaultHandler				; for SWI47
	.word SWIDefaultHandler				; for SWI48
	.word SWIDefaultHandler				; for SWI49
	.word SWIDefaultHandler				; for SWI50
	.word SWIDefaultHandler				; for SWI51
	.word SWIDefaultHandler				; for SWI52
	.word SWIDefaultHandler				; for SWI53
	.word SWIDefaultHandler				; for SWI54
	.word SWIDefaultHandler				; for SWI55
	.word SWIDefaultHandler				; for SWI56
	.word SWIDefaultHandler				; for SWI57
	.word SWIDefaultHandler				; for SWI58
	.word SWIDefaultHandler				; for SWI59
	.word SWIDefaultHandler				; for SWI60
	.word SWIDefaultHandler				; for SWI61
	.word SWIDefaultHandler				; for SWI62
	.word SWIDefaultHandler				; for SWI63

SWIDefaultHandler:
	B SWIDefaultHandler

ISR_SWI_end:
    ; restore critical context
    MSR SPSR_cf, R11					; Restore the SPSR from R11
    LDMFD SP!, {R1-R12, LR}				; Restore working registers and Link register (do not restore R0 to preserve return value)

    ; Return after handling the interrupt
    MOVS PC, LR
