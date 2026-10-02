DEF wPayloadStart EQU $D280
DEF sStorage EQU $BE2E
DEF sStorageBank EQU $03
DEF wTMEntryPoint EQU $DA10
DEF TM17EntryPoint EQU $DA47
DEF hActiveSRAMBank EQU $FFFE
DEF wMailWriter = $D9C0
DEF MailWriterBCDOffset = $07
DEF AceTMId = $CE
DEF mailWriterComplement EQU $9e

; Values

DEF BattleTMId EQU $D0
DEF QuitFirstLetterId EQU $92

DEF SpecialCallIndex EQU $A5
DEF SpecialCallPointer EQU $DAE5
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
