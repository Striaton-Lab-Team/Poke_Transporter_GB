DEF wPayloadStart EQU $D280
DEF sStorage EQU $BE2E
DEF sStorageBank EQU $03
DEF wTMEntryPoint EQU $DA10
DEF TM17EntryPoint EQU $DA47
DEF hActiveSRAMBank EQU $FFFE
DEF wMailWriter = $D9C0
DEF MailWriterBCDOffset = $11
DEF AceTMId = $CE
DEF mailWriterComplement EQU $61

; Values

DEF BattleTMId EQU $D0
DEF QuitFirstLetterId EQU $90

DEF SpecialCallIndex EQU $9C
DEF SpecialCallPointer EQU $DA21
DEF SpecialCallCode EQU $DA15
DEF SpecialCallACEPayloadA EQU $D6F6
DEF SpecialCallACEPayloadB EQU $DA93
DEF SpecialCallACEPayloadC EQU $DAAB

DEF MAILWRITER_SIZE = $32

DEF TREECKO_NAME EQUS "TREECKO@@@@"
DEF GROVYLE_NAME EQUS "GROVYLE@@@@"
DEF SCEPTILE_NAME EQUS "SCEPTILE@@@"
DEF TORCHIC_NAME EQUS "TORCHIC@@@@"
DEF TextHeight EQU 0x1
