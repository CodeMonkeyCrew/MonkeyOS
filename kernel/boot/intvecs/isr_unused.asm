	.global _ISR_UNUSED

	.retain ".isr"
	.sect ".isr"
_ISR_UNUSED:
	B _ISR_UNUSED
