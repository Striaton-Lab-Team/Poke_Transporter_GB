DEF wPayloadStart EQU $DD55
DEF sStorage EQU $BE70
DEF sStorageBank EQU $03
DEF wTMEntryPoint EQU $D418
DEF hActiveSRAMBank EQU $FFFE
DEF wMailWriter = $D6F2
DEF MailWriterBCDOffset = $11
DEF AceTMId = $E1
DEF mailWriterComplement EQU $8f

; Values

DEF BattleTMId EQU $E1
DEF QuitFirstLetterId EQU $84

DEF SpecialCallIndex EQU $33
DEF SpecialCallPointer EQU $D830
DEF SpecialCallACEPayloadA EQU $D746
DEF SpecialCallACEPayloadB EQU $D76D
DEF SpecialCallACEPayloadC EQU $D785

DEF MAILWRITER_SIZE = $32

DEF TREECKO_NAME EQUS "TREECKO@@@@"
DEF GROVYLE_NAME EQUS "GROVYLE@@@@"
DEF SCEPTILE_NAME EQUS "SCEPTILE@@@"
DEF TORCHIC_NAME EQUS "TORCHIC@@@@"
DEF TextHeight EQU 0x1
