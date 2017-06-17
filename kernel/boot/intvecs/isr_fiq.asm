	.global _ISR_FIQ

	.retain ".isr"
	.sect ".isr"
_ISR_FIQ:
	B _ISR_FIQ
