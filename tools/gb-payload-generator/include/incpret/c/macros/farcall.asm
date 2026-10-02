; Far calls to another bank

MACRO farcall ; bank, address
	ld a, \1Bank
	ld hl, \1
	rst FarCall
ENDM

MACRO callfar ; address, bank
	ld hl, \1
	ld a, \1Bank
	rst FarCall
ENDM

MACRO homecall
	ldh a, [hROMBank]
	push af
	ld a, \1Bank
	rst Bankswitch
	call \1
	pop af
	rst Bankswitch
ENDM
