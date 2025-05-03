; original: CC6303/lib6800/_memset.s

	.export _memset

	.code

_memset:
	tsx
	ldab	5,x
	stab	@tmp
	ldaa	6,x		; length
	ldab	7,x
	ldx	2,x		; destination
	bsr	nextblock
	tsx
	jmp	ret6

nextblock:
	tsta
	beq	tailset
	; Set 256 bytes
	pshb
	psha
	clrb
	bsr	tailset
	pula
	pulb
	deca
	bra	nextblock

tailset:
	ldaa	@tmp
clearloop:
	staa	,x
	inx
	decb
	bne	clearloop
	rts
