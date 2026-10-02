; Contains a list of symbols that end up being shared across multiple versions and languages within gen 1.

DEF SerialInterrupt = 0x58
DEF Start = 0x100
DEF GLOBAL_CHECKSUM = 0x014E

DEF Bankswitch = 0x3E84

DEF wMuteAudioAndPauseMusic = 0xC002

DEF wTileMap = 0xC3A0
DEF wSerialPartyMonsPatchList = 0xC508
DEF wSerialEnemyMonsPatchList = 0xC5D0
DEF wSerialEnemyMonsPatchList_GS = 0xC5D0
DEF wSerialEnemyMonsPatchList_C = 0xC6D0
DEF wSerialOtherGameboyRandomNumberListBlock = 0xCD91
DEF wPrinterConnectionOpen = 0xD499
DEF wSerialEnemyDataBlock = 0xD893
DEF CHECKSUMPACKET_SIZE = 4

DEF RNGSEED_SIZE = 0x0A
DEF PAYLOAD_SIZE = 0x01AA
DEF PATCHLIST_SIZE = 0xC9

; RAM patch definitions

DEF SERIAL_EXCHANGEBYTES_OFFSET = 0xBF1
DEF SERIAL_EXCHANGEBYTES_SEARCH = 0xE0AB

DEF PRINTWAITINGTEXT_OFFSET = 0x49ED
DEF PRINTWAITINGTEXT_SEARCH = 0x216B