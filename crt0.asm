	.setcpu		6800
	.code
	.export _exit
start:
	sei
	ldaa	#$7e
	staa	jmptmp
	lds	#$afff
	ldx	#0
	stx	@zero
	inx
	stx	@one
	ldx	#__bss_size
	beq	nobss
	clra
	clrb
	subb	#<__bss_size
	sbca	#>__bss_size
	ldx	#__bss
clear_bss:
	clr	,x
	inx
	incb
	bne	clear_bss
	inca
	bne	clear_bss
nobss:
	jsr	_main
_exit:
loop:
	jmp	loop
