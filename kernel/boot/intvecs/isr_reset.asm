	.global _ISR_RESET

	.retain ".isr"
	.sect ".isr"
_ISR_RESET:
	B _ISR_RESET
