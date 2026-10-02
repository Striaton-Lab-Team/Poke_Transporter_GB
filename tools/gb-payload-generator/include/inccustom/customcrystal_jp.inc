DEF wPayloadStart EQU $D2B1
DEF sStorage EQU $BE2E
DEF sStorageBank EQU $03
DEF wTMEntryPoint EQU $DA10
DEF TM17EntryPoint EQU $DA47
DEF hActiveSRAMBank EQU $FFFE
DEF wMailWriter = $D9B3
DEF MailWriterBCDOffset = $05
DEF AceTMId = $CE
DEF mailWriterComplement EQU $e9


; Values

DEF BattleTMId EQU $D0
DEF QuitFirstLetterId EQU $D4

DEF SpecialCallIndex EQU $68
DEF SpecialCallPointer EQU $DEBA
DEF SpecialCallCode EQU $DA15
DEF SpecialCallACEPayloadA EQU $D6E9
DEF SpecialCallACEPayloadB EQU $DA86
DEF SpecialCallACEPayloadC EQU $DA9E

DEF MAILWRITER_SIZE = $32

; Names

DEF TREECKO_NAME EQUS "キモリ@@@"
DEF GROVYLE_NAME EQUS "ジュプトル@"
DEF SCEPTILE_NAME EQUS "ジュカイン@"
DEF TORCHIC_NAME EQUS "アチャモ@@"
DEF TextHeight EQU 0x2
