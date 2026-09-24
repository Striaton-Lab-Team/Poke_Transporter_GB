; Contains a list of symbols that end up being shared across multiple versions and languages within gen 2.

DEF NULL = 0x0
DEF FarCall = 0x8
DEF Bankswitch = 0x10
DEF Jumptable = 0x28
DEF SerialInterrupt = 0x58
DEF Start = 0x100
DEF GLOBAL_CHECKSUM = 0x014E
DEF ClearScreen = 0x0F00
DEF PlaceWaitingTextBank = 0x01
DEF PlaceWaitingText = 0x4000

DEF wMusicFadeID = 0xC1A9

DEF wTileMap = 0xC3A0
DEF wSerialPartyMonsPatchList = 0xC508
DEF wSerialEnemyMonsPatchList = 0xC5D0
DEF wSerialEnemyMonsPatchList_GS = 0xC5D0
DEF wSerialEnemyMonsPatchList_C = 0xC6D0
DEF wSerialOtherGameboyRandomNumberListBlock = 0xCD91
DEF UNIVERSALALIGNED = 0xC800
DEF CHECKSUMPACKET_SIZE = 4

DEF hCGB_C = 0xFFE6
DEF hCGB_GS = 0xFFE8

DEF RNGSIZE = 0x11
DEF LINKDATASIZE = 0x1C2
DEF PATCHLIST = 0xC8

DEF SerialOffset_GS = 0x66
DEF SerialOffset_C = 0x70