DEF wPayloadStart EQU $DCC6
DEF sStorage EQU $BE70
DEF sStorageBank EQU $03
DEF wTMEntryPoint EQU $D418
DEF hActiveSRAMBank EQU $FFFE
DEF wMailWriter = $D6E5
DEF MailWriterBCDOffset = $00
DEF AceTMId = $E1
DEF mailWriterComplement EQU $4c

; Values

DEF BattleTMId EQU $E1
DEF QuitFirstLetterId EQU $D4

DEF SpecialCallIndex EQU $DB
DEF SpecialCallPointer EQU $D1CF
DEF SpecialCallACEPayloadA EQU $D739
DEF SpecialCallACEPayloadB EQU $D760
DEF SpecialCallACEPayloadC EQU $D778

DEF MAILWRITER_SIZE = $32

; Names

DEF TREECKO_NAME EQUS "キモリ@@@"
DEF GROVYLE_NAME EQUS "ジュプトル@"
DEF SCEPTILE_NAME EQUS "ジュカイン@"
DEF TORCHIC_NAME EQUS "アチャモ@@"
DEF TextHeight EQU 0x2
