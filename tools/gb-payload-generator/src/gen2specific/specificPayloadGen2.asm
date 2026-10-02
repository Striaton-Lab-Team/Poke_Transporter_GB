INCLUDE "include/constants/gen2specific.asm"

SECTION "Main", ROM0
Main:
	db 0xFD
LOAD "Payload", WRAM0[SPECIFICPAYLOAD]
Payload:
.loopTransfer
	ld hl, PACKET_RECEIVE ; new place to store incoming packets. We use this location since 0xC6DC is vulnerable to overwrites from SaveBox.
.searchCounter
	ld a, [hli]
	bit 7, a
	jr nz, .searchCounter ; first byte should be a packet counter, with a value between 0x00-0x7F. Also skips preamble bytes.
	ld [PACKET_RECEIVE], a ; Put this at the start of the received data, to ensure we'll be sending it back if we're running a command.
	push af ; we'll retrieve this later
	ld a, [hli] ; load command byte
	ld b, [hl] ; load argument byte 1
	inc hl
	ld c, [hl] ; load argument byte 2
	inc hl
	push hl
	ld hl, CommandTable
	cp a, (CommandTable.end - CommandTable) / 2 ; number of valid commands.
	jr nc, .skip
	rst JumpTable ; if command not in table, send data to PTGB instead. Afterwards, request another packet from PTGB.
.skip
	pop hl ; hl should now point to the pointer that PTGB wants us to read data from.
	ld a, [hli]
	ld h, [hl]
	ld l, a
	ld de, PACKET_SEND + 3
	pop bc ; sneaky way to load the counter into the checksum
	push de
	ld c, 1
.loop
	ld a, [hli]
	rra ; c flag is always 0 here, except if a failure state was detected in a command we ran this cycle. Failure state means 1st data byte > 0x80
	push af ; store new c flag for later
	inc de
	ld [de], a
	add a, b 
	ld b, a ; calculates checksum
	pop af
	ld a, c ; register c stores LSB bytes that were extracted using rra
	adc a, a ; shifts a left, then adds the current carry flag to a. If shifted for a total of 8 times, a will overflow and we escape the loop.
	ld c, a
	jr nc, .loop
	
	add a, b
	add a, h
	add a, l
	res 7, a ; ensure that the checksum is never equal to 0xFE
	inc de ; hl should now point to 0xC8DC, corresponding to the packet counter of the last received packet.
	pop hl
	ld [hld], a
	ld a, c
	and 0x0F ; extract the lower nybble.
	ld [hld], a ; 4 LSB get stored here.
	xor c ; I love xor magic, this stores the higher nybble in a.
	ld [hld], a ; 4 MSB get stored here, hl now points to 0xC8D0, where the next packet will be sent from.
	ld a, [de] 
	ld [hl], a ; write counter
	ld c, 12
	call .changeInterruptsAndCommunicate
	jr .loopTransfer
.changeInterruptsAndCommunicate
	call 0xC86B ; leftover from the universal payload, only allow serial interrupt and call Serial_ExchangeBytes
	ld a, IE_JOYPAD | IE_SERIAL | IE_TIMER | IE_VBLANK ; enable vblank interrupt so that a sound effect can play
	ldh [rIE], a
	ret
VerifySecondaryPayload: ; checks if payload matches expected size and passes verification.
	ld c, b
	ld de, SECONDARYPACKET
	ld hl, PACKET_RECEIVE
	push de
	call Payload.changeInterruptsAndCommunicate
	pop hl
	ld e, c
.skipPreambleAndFF
	ld a, [hli]
	inc a
	jr nz, .skipPreambleAndFF
	ld c, [hl] ; c now equals the size of the payload's data
	inc hl
.checksumLoop	
	ld a, [hli]
	ld [de], a
	inc de
	add a, b
	ld b, a
	dec c
	jr nz, .checksumLoop ; calculates checksum and aligns payload at the same time
	ld l, c
	and a
	ret z
	pop hl
	scf
	ret
ReloadCurrentBox:
; No longer needed due to better ROM structure
TransferPokemon: ; possible usecase: use second argument byte to change box?
	ld [wPokemonWithdrawDepositParameter], a ; a non-zero value in this address indicates we'll be removing stuff from the current active box.
	ld a, c
	ld [wCurBox], a
	push bc
	ld de, ReplaceTextBox.transferringText
	call ReplaceTextBox.writeOwnString
	farcall LoadBox
	pop bc
	call VerifySecondaryPayload
.removalLoop ; implementation of Gears' removal loop
	ld a, [hli]
	cp a, 0xFF
	jr z, .saveBox ; possible to optimize this jr out?
	push hl
	ld [wCurPartyMon], a
	farcall RemoveMonFromPartyOrBox
	pop hl
	jr .removalLoop	
.saveBox
	ld hl, ReplaceTextBox.writeCommunicating ; the sound of the game saving will provide enough time to display this on screen
	push hl ; ensures that new text is only written AFTER we save the game
	farcall SaveBox
	ret
ModifySRAMAccess: ; opens SRAM bank to the bank described in argument byte 1.
	ld h, b
	ld [hl], h
	ld h, 0x40
	ld [hl], c
	ret
RunSecondaryPayload: ; loads new payload of size b, aligns it, verifies it, then executes it.
	call VerifySecondaryPayload
	jp hl
ReplaceTextBox:
	hlcoord 2, 10
	ld bc, TextHeight << 8 | 14 ; Korean and Japanese text can have a height of 2 tiles
	ld a, 0x10 ; predef ID for LinkTextboxAtHL
	call Predef
.writeCommunicating
	ld de, .communicatingText
.writeOwnString
	hlcoord 3, 11
	call PlaceString ; ensures multiple vblank occur, which will result in the screen updating and displaying the new text.
	ld c, 3
	jp DelayFrames
.communicatingText:
	db "COMMUNICATING!@"
.transferringText:
	db "TRANSFERRING! @"
CommandTable:
	dw ReplaceTextBox
	dw TransferPokemon
	dw Reset
	dw ModifySRAMAccess
	dw RunSecondaryPayload
.end
ds 255 - (CommandTable.end - Payload), 0
ENDL
