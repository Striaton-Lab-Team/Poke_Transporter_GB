INCLUDE "include/macros/const.asm"

INCLUDE "include/constants/charmap.asm"
INCLUDE "include/constants/serial_constants.asm"
INCLUDE "include/constants/pokemon_constants.asm"
INCLUDE "include/constants/gen1universal.asm"
INCLUDE "include/constants/gen1specific.asm"
INCLUDE "include/constants/hardware.inc"

SECTION "Payload", ROM0
Payload:
LOAD "RNGSeed", WRAM0[wSerialOtherGameboyRandomNumberListBlock - 1]
RNGSeed:
	ds 7, 0
	jp SerialPatchPreamble.end
.end
ds RNGSEED_SIZE - (RNGSeed.end - RNGSeed), 0
ENDL


LOAD "PartyData", WRAMX[wSerialEnemyDataBlock - 1]
PartyDataPreamble:
	ds 7, SERIAL_PREAMBLE_BYTE
.end

PartyData:
	ds 6, '@'
	ds 5, DITTO ; on JP, this becomes part of party data. On other versions, this becomes part of the name.
.partyCount
	db DITTO
.partyList
	ds 9, DITTO ; Along with previous values, ensures that ditto's species names will be visible on screen.
	ds 4, 0xE0 ; Blank name on JP, leaves patch list data as intact as possible.
	ds 37, MEW ; Mew is the best available filler, only taking 3 normal text chars of space on every language.
	db 0xBF ; Only affects INT Yellow, places FA 60 D3 47 FA 67 D3 CD 7E 47 at 0xC83E. The first character is later read as a pointer.
	ds 33, MEW
	db 0xC6 ; Only affects INT Yellow, places 19 2A 66 6F E9 FD 47 C9 48 A6 at 0xCAE6. This fetches a pointer from 0xC83D and jumps towards it, effectively jumping to 0xFA0E.
	ds 29, MEW
	db 0    ; Only affects EN Yellow, places D0 04 19 3D 18 F9 93 91 80 88 8D 84 91 8F 8E 8A BA 21 F0 C6 at 0xCD3D, covering up 0xCD4F and 0xCD50 with safe values.
	db 0xC6 ; Only affects EN Yellow, places 19 2A 66 6F E9 FD 47 C9 48 A6 at 0xCD52. This fetches a pointer from 0xCD95 and jumps towards it, effectively jumping to 0xFA1C.
	ds 2, MEW
	db 0xE3 ; Blank on non-JP RB. This nickname would normally be placed at 0xCD8E, which must be kept clear for the RB payload to work properly.
	ds 5, MEW
	ds 3, 0xE0 ; Blank on JP, keeps JP as clean as possible. One of these values also prevents the game from becoming unresponsive on non-JP Yellow.
	db 0xE3 ; Only affects EN Yellow, places 07 FA 71 C9 FE FF 28 1C FA 70 at 0xCE42. The 8th and 9th character are read as a pointer.
	ds 18, 0xE0
	db MEW ; Prevents sound corruption on non-JP versions
	ds 27, 0xE0
	db MR_MIME ; Affects the stack on JP versions, following species names will instead be written from address offset 0xD819.
	ds 17, 0xE0
	db RAICHU ; Same name on all non-JP versions, prevents wLetterPrintingDelayFlags from being overwritten to values that would slow down text printing.
	ds 28, 0xE0 ; On non-JP versions, the 0xE0 at offset 16 will be overwritten to a 0xFE. On EN Yellow, this enables serial interrupt ACE using printer opcode 0xFA, with an effect pointer at 0xCD4F
	db PIDGEOTTO ; On JP, the diacritics on the 1st and 2nd characters of this species name (ピジョン) will overwrite the stack, leading to a return to 0xE5E4.
	ds 51, MEW
	db 0xE4 ; Blank on RB, places C9 FE 81 20 15 FA 71 C9 FE 00 at 0xD9AA, the latter 6 characters are then reinterpreted as species IDs. This causes EN Yellow to get stuck printing 0xC9's name, while ensuring the stack smash species ID remains intact. WHen the player presses A twice on this state, the game will execute code starting from  (due to the placement of 0xFE's species name intercepting the expected jump towards 0xFA0E).
	ds 62, MEW
	jr .skip ; On EN Y, this is placed at 0xD9EC, and safely causes a jump to a safer jump.
	ds 14, MEW
	db 0xFC ; Used as a stack smasher on all non-JP versions. On RB, this jumps execution to 0xCD8A. On Y, this jumps execution to 0xCAE2.
	ds 26, MEW
	jp SerialPatchPreamble.end ; Placed at 0xFA1C on non-JP, non-EN versions of Y.
	ds 2, MEW
.skip
	xor a
	ld [wPrinterConnectionOpen], a
	ld b, 1
	ds 3, 0
	call SerialInterrupt
	ld hl, SerialPatchPreamble.end
	jp Bankswitch ; Bankswitch in EN Yellow. PrintWaitingText is located in ROM bank 01, so we must switch to correctly find it.
.end
ds PAYLOAD_SIZE - (PartyData.end - PartyDataPreamble), 0
ENDL

LOAD "serialPatch", WRAM0[wSerialEnemyMonsPatchList]
SerialPatchPreamble:
	ds 5, SERIAL_PREAMBLE_BYTE
	db 0xCA ; places 0xFE in the correct place for the non-JP payload. Due to differences in name sizes, the 0xFE is placed at a -5 offset on the JP versions. This does not cause issues.
	ds 2, SERIAL_PATCH_LIST_PART_TERMINATOR
.end
ENDL

SerialPatchListPayload:
	ds 1, 0
	ld bc, SCREEN_AREA - 1
	ld hl, wTileMap + SCREEN_AREA - 1 ; bottom right screen tile, will always be 0x7F
.clearScreenLoop ; replace all tiles on screen with blank tiles.
	dec bc
	ld a, [hld]
	ld [hl], a
	jr .skip
	ds 1, 0 ; this value should land on 0xC5E4, the JP entry point4
	jr SerialPatchListPayload + 1 ; not overwritten on JP
	ds 3, 0
.skip
	ld a, b
	or c
	jr nz, .clearScreenLoop
	ld hl, SerialPatchListAligned + 1
	inc a	; possibly overwritten on JP
	ld [wMuteAudioAndPauseMusic], a ; always overwritten on JP
	ds 1, 0
	push hl
	pop de
.findOpcode ; yes, we're cramming this in!!
	ld a, [hli]
	push af
	ld [wMuteAudioAndPauseMusic], a ; always overwritten on non-JP
	ds 1, 0
	pop af
	and a, a ; we're skipping all 0 values
	jr z, .findOpcode
.alignPayload
	ld [de], a
	inc e
	ld a, [hli]
	jr nz, .alignPayload ; align stuff until we hit 0xC6xx
.end
LOAD "serialPatchAligned", WRAM0[0xC607 - 1]
SerialPatchListAligned:
	ds 1, 0
	ld hl, PRINTWAITINGTEXT_OFFSET
	ld bc, PRINTWAITINGTEXT_SEARCH
	ds 4, 0
	call .findPointerAndPatch
.callPrintWaitingText ; use the built in ROM function to print the "waiting" text
	call .callPatchedPointer
	ld hl, SERIAL_EXCHANGEBYTES_OFFSET
	ld bc, SERIAL_EXCHANGEBYTES_SEARCH
	call .findPointerAndPatch
	ds 5, 0
.sendChecksum
	ld hl, GLOBAL_CHECKSUM ; locate cartridge checksum
	ld a, [hli]
	ld b, [hl]
	ld hl, PACKET_SEND + 1 ; prepare to send cartridge checksum
	ld [hli], a
	ld [hl], b
	inc hl
	add a, b
	res 7, a
	ld [hl], a	; place checksum
	ds 5, 0 ; expected result is FD [16 bit cartridge checksum] [8 bit safety checksum]. There are no valid cartridge checksums containing 0xFD or 0xFE.
	ld l, LOW(PACKET_SEND) ; send checksum data from this address
	ld de, SPECIFICPAYLOAD ; receive payload at this address
	ld c, CHECKSUMPACKET_SIZE
	call .callSerial_ExchangeBytes ; send checksum to PTGB, bc = 0000 on exit
	ld e, c
	push de
	dec c ; c = 0xFF
	call .callSerial_ExchangeBytes ; receive specific payload from PTGB
	ds 4, 0
	pop hl
	ld e, c
.findNotPreamble ; we can't be sure if the payload arrived in the exact desired position. Because of this, let's align the payload.
	ld a, [hli]
	cp SERIAL_PREAMBLE_BYTE
	jr z, .findNotPreamble
.alignPayload ; now that we have found the first non-preamble value, let's align it properly.
	ld [de], a
	inc e ; we're only aligning stuff within the 0xC800 space. Exit when de = 0xC800
	ld a, [hli]
	jr nz, .alignPayload
	ld a, [de] ; check first non-preamble byte of payload. If 00, resend checksum (incorrect calculated checksum). If FF, an error has occured (correct calculated checksum, but the cartridge checksum is not recognized), so safely reset instead. If any other values, jump to new payload!
	and a, a
	jr z, .sendChecksum
	ds 4, 0
	push de ; push 0xC800 on the stack for the next ret
	inc a
	ld a, IE_SERIAL | IE_TIMER | IE_VBLANK
	ldh [rIE], a
	ret nz
	ld a, 0xC3
	ld hl, Start - 1
.loopUntilJumpInit
	inc hl
	cp a, [hl]
	jr nz, .loopUntilJumpInit
	ds 4, 0
	jp hl
.findPointerAndPatch ; searches for specific opcodes from a certain offset and places the resulting pointer at a specific location
; b = first opcode to search for
; c = second opcode to search for
; de = address to write pointer to
; hl = offset to start searching from
; return destination pointer in hl, made to minimize amount of M-cycles taken
	ld de, .callPatchedPointer + 1
.findPointerAndPatchLoop
	ld a, [hli]
	cp b
	jr nz, .findPointerAndPatchLoop
	ld a, [hl] ; we can't ldd here, otherwise we could get stuck in an infinite loop
	cp c
	jr nz, .findPointerAndPatchLoop
	dec hl
	ld a, l
	ld [de], a
	inc de
	ds 4, 0
	ld a, h
	ld [de], a
	ret
.callSerial_ExchangeBytes
	ld [hl], SERIAL_PREAMBLE_BYTE
	ld b, 0
	ld a, IE_SERIAL
	ldh [rIE], a
.callPatchedPointer
	db 0xC3
.end
ds PATCHLIST_SIZE - (SerialPatchListAligned.end - SerialPatchListAligned) - (SerialPatchListPayload.end - SerialPatchListPayload) - (SerialPatchPreamble.end - SerialPatchPreamble) - 1, 0
ENDL

; FD FD FD FD FD CA FF FF 00 01 67 01 21 07 C5 0B
; 3A 77 18 06 00 18 F2 00 00 00 78 B1 20 F1 21 07
; C6 00 00 00 00 00 E5 D1 2A F5 00 00 00 00 F1 A7
; 20 F6 12 1C 2A 20 FB 00 21 ED 49 01 6B 21 00 00
; 00 00 CD 7B C6 CD 93 C6 21 F1 0B 01 AB E0 CD 7B
; C6 00 00 00 00 00 21 4E 01 2A 46 21 D1 C5 22 70
; 23 80 CB BF 77 00 00 00 00 00 2E D0 11 08 C5 E5
; 01 05 00 D5 CD 91 C6 D1 E1 D5 00 00 00 00 0E C8
; CD 91 C6 E1 E5 E5 D1 2A FE FD 28 FB 12 00 00 00
; 00 00 1C 2A 20 F6 E1 7E A7 28 BB E5 3C C0 F0 FE
; 18 04 00 00 00 00 C6 10 C3 00 01 11 94 C6 2A B8
; 20 FC 7E B9 20 F8 00 00 00 00 2B 7D 12 13 7C 12
; C9 3E 01 C3 93 C6 00 00


	;JP:
; FD FD FD FD FD FF FF 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 A0 8F A1 AB 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
; 00 00 00 00 00 00 00 00  
	;INT:
; FD FD 8C 84 96 CA FF FF 00 00 00 00 00 00 00 00   ;  FD FD FD FD FD CA FF FF 00 00 00 00 AF 21 99 D4
; 00 00 00 00 00 00 8C 84 96 __ 00 00 00 00 00 00   ;  22 22 00 00 00 00 00 00 00 01 68 01 21 A0 C3 00
; 00 A0 8F A1 AB __ 00 00 00 00 8C 84 96 __ 00 00   ;  00 00 00 00 00 3E 7F 22 0B 00 00 00 00 78 B1 20
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 8C 84   ;  F4 21 ED 49 11 11 C6 01 6B 21 CD 79 C6 00 00 00
; 96 __ 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ;  00 CD 10 C6 21 F1 0B 11 8E C6 01 AB E0 CD 79 C6
; 00 00 8C 84 96 __ 00 00 00 00 00 00 00 00 00 00   ;  26 01 00 00 00 2E 4E 2A 46 21 D1 C5 22 70 23 80
; 00 00 00 00 00 00 8C 84 96 __ 00 00 00 00 00 00   ;  CB BF 77 21 D0 C5 00 00 00 11 0A C5 E5 D5 01 05
; 00 00 00 00 00 00 00 00 00 00 8C 84 96 __ 00 00   ;  00 CD 8B C6 D1 E1 01 C8 00 C5 00 00 00 D5 CD 8B
; 00 00 00 00 00 00 00 00 00 00 00 00 00 00 8C 84   ;  C6 E1 C1 23 E5 E5 E5 D1 2A FE FD 28 FB 12 00 00
; 96 __ 00 00 00 00 00 00 00 00 00 00 00 00 00 00   ;  00 13 0B 78 B1 2A C2 5C C6 E1 7E A7 28 B2 3C C0
; 00 00 8C 84 96 __ 00 00 00 00 00 00 00 00 00 00   ;  F0 FE 00 00 00 C6 10 C3 00 01 2A B8 20 FC 7E B9
; 00 00 00 00 00 00 8C 84 96 __ 00 00 00 00 00 00   ;  20 F8 2B 7D 12 13 00 00 00 7C 12 C9 3E 01 C3 8B
; 00 00 00 00 00 00 00 00                           ;  C6 00 00 00 00 00 00 00 00