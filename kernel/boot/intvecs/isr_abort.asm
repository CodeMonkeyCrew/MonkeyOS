	.global _ISR_ABORT

	.retain ".isr"
	.sect ".isr"
_ISR_ABORT:
	B _ISR_ABORT
