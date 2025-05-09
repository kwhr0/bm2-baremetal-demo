; original: CC6303/lib6800/_memcpy.s

;
;	There isn't a nice way to do this on 680x/630x.
;
;
	.export _memcpy

	.setcpu 6800
	.code

_memcpy:
	tsx
	ldaa	2,x
	ldab	3,x
	staa	@tmp
	stab	@tmp+1
	ldaa	6,x		; length
	oraa	7,x
	beq	nocopy
	ldaa	6,x
	ldab	7,x
	ldx	4,x		; src
	bsr	nextblock
	tsx
nocopy:
	ldaa	6,x
	ldab	7,x
	jmp	ret6

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
	ldx	@tmp
	staa	,x
	inx
	stx	@tmp
	ldx	@tmp2
	decb
	bne	tailcopy
	rts
