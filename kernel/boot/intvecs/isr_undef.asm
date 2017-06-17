	.global _ISR_UNDEF

	.retain ".isr"
	.sect ".isr"
_ISR_UNDEF:
	B _ISR_UNDEF
