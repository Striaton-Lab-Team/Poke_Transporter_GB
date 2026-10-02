DEF wPayloadStart EQU $D280
DEF sStorage EQU $BE2E
DEF sStorageBank EQU $03
DEF wTMEntryPoint EQU $DA10
DEF TM17EntryPoint EQU $DA47
DEF hActiveSRAMBank EQU $FFFE
DEF wMailWriter = $D9C0
DEF MailWriterBCDOffset = $11
DEF AceTMId = $CE
DEF mailWriterComplement EQU $8d

; Values

DEF BattleTMId EQU $D0
DEF QuitFirstLetterId EQU $84

DEF SpecialCallIndex EQU $EB
DEF SpecialCallPointer EQU $DAAA
DEF SpecialCallCode EQU $DA93
DEF SpecialCallACEPayloadA EQU $D6F6
DEF SpecialCallACEPayloadBPrefix EQU $DA15
DEF SpecialCallACEPayloadB EQU $DA1D
DEF SpecialCallACEPayloadC EQU $DA35

DEF MAILWRITER_SIZE = $32

DEF TREECKO_NAME EQUS "TREECKO@@@@"
DEF GROVYLE_NAME EQUS "GROVYLE@@@@"
DEF SCEPTILE_NAME EQUS "SCEPTILE@@@"
DEF TORCHIC_NAME EQUS "TORCHIC@@@@"
DEF TextHeight EQU 0x1
