; original: chibicc-6800-v1/crt0.s

;
;	A dummy minimal crt0.s for now
;

	.zp
	.export bp
	.export long
bp:	.word	0
long:	.word	0
	.word	0
;
	.export	rdi
rdi:	.word	0

	.code
	.export _exit
start:
	sei
	lds	#$afff
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
