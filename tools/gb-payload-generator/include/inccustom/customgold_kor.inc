DEF wPayloadStart EQU $DE52
DEF sStorage EQU $BE70
DEF sStorageBank EQU $03
DEF wTMEntryPoint EQU $D418
DEF hActiveSRAMBank EQU $FFFE
DEF wMailWriter = $D6F2 ; not relevant for KOR
DEF MailWriterBCDOffset = $07
DEF AceTMId = $CE
DEF mailWriterComplement EQU $13

; Values

DEF BattleTMId EQU $F1
DEF QuitFirstLetterId EQU $C0

DEF SpecialCallIndex EQU $1F
DEF SpecialCallPointer EQU $D4CD
DEF SpecialCallACEPayloadA EQU $D7D9
DEF SpecialCallACEPayloadB EQU $D800
DEF SpecialCallACEPayloadC EQU $D818

DEF MAILWRITER_SIZE = $32

DEF TREECKO_NAME EQUS "나무지기@@@"
DEF GROVYLE_NAME EQUS "나무돌이@@@"
DEF SCEPTILE_NAME EQUS "나무킹@@@@@"
DEF TORCHIC_NAME EQUS "아차모@@@@@"
DEF TextHeight EQU 0x2
