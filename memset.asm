; original: CC6303/lib6800/_memset.s

	.export _memset

	.code

_memset:
	staa	@tmp2		; dst pointer
	stab	@tmp2+1
	tsx
	ldab	3,x		; fill data
	stab	@tmp1
	ldaa	4,x		; length
	ldab	5,x
	ldx	@tmp2
	bsr	nextblock
	tsx
	ldaa	@tmp2
	ldab	@tmp2+1
	rts

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
	ldaa	@tmp1
clearloop:
	staa	,x
	inx
	decb
	bne	clearloop
	rts
