; original: CC6303/lib6800/_memcpy.s

;
;	There isn't a nice way to do this on 680x/630x.
;
;
	.export _memcpy

	.setcpu 6800
	.code

_memcpy:
	staa	@tmp1		; dst pointer
	stab	@tmp1+1
	tsx
	ldaa	4,x		; length
	oraa	5,x
	beq	nocopy
	ldaa	4,x
	ldab	5,x
	ldx	2,x		; src pointer
	bsr	nextblock
	tsx
nocopy:
	ldaa	@tmp1
	ldab	@tmp1+1
	rts

nextblock:
	tsta
	beq	tailcopy
	; Copy 256 bytes repeatedly until we get to the leftovers
	pshb
	psha
	clrb
	bsr	tailcopy
	pula
	pulb
	deca
	bra	nextblock

tailcopy:
	ldaa	,x
	inx
	stx	@tmp2
	ldx	@tmp1
	staa	,x
	inx
	stx	@tmp1
	ldx	@tmp2
	decb
	bne	tailcopy
	rts
