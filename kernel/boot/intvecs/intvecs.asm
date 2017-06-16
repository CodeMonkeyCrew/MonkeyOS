	.global _ISR_RESET
	.global _ISR_UNDEF
	.global _ISR_SWI
	.global _ISR_PREFETCH
	.global _ISR_ABORT
	.global _ISR_UNUSED
	.global _ISR_IRQ
	.global _ISR_FIQ

	.retain ".intvecs"
	.sect ".intvecs"
	B _ISR_RESET
	B _ISR_UNDEF
	B _ISR_SWI
	B _ISR_PREFETCH
	B _ISR_ABORT
	B _ISR_UNUSED
	B _ISR_IRQ
	B _ISR_FIQ

_ISR_RESET:
	B _ISR_RESET
