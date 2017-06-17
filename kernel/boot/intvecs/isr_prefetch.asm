	.global _ISR_PREFETCH

	.retain ".isr"
	.sect ".isr"
_ISR_PREFETCH:
	B _ISR_PREFETCH
