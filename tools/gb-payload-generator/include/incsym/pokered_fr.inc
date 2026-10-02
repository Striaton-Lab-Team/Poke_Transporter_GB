; ROM functions, highly variable between versions

DEF CopyData EQU 0xB5
DEF ClearVram EQU 0x2000
DEF GiveItem EQU 0x3E4B

DEF RemovePokemon EQU 0x393C
DEF JoypadLowSensitivity EQU 0x384E
DEF PrintBCDNumber EQU 0x15CA

; wRAM symbols, usually offset by constant amounts between versions/languages

DEF wTileMap EQU 0xC3A0
DEF wListScrollOffset EQU 0xCC36
DEF wBagSavedMenuItem EQU 0xCC2C
DEF wStringBuffer EQU 0xCF50
DEF wPartyCount EQU 0xD168
DEF wMissableObjectFlags EQU 0xD5AB
DEF wNumBagItems EQU 0xD322
DEF wPokedexOwned EQU 0xD2FC
DEF wBoxCount EQU 0xDA85
DEF wDestinationWarpID EQU 0xD434
DEF wStatusFlags3 EQU 0xD732

; hRAM symbols, usually only vary between RB and Y

DEF hWarpDestinationMap EQU 0xFF8B
DEF hWY EQU 0xFFB0
DEF hJoy5 EQU 0xFFB5
DEF LoadCurrentBoxDataBank EQU 0x1C
DEF LoadCurrentBoxData EQU 0x7661
DEF SaveCurrentBoxData EQU 0x77B3
DEF Bankswitch EQU 0x35F3
DEF CallFunctionInTable EQU 0x3DB4
DEF SoftReset EQU 0x1F45
DEF wCurrentMenuItem EQU 0xCC26
DEF wWhichPokemon EQU 0xCF97
DEF wRemoveMonFromBox EQU 0xCF9A
DEF Serial_ExchangeBytes EQU 0x216B
DEF ClearScreen EQU 0x190C
DEF PlaceString EQU 0x1952
DEF CableClub_TextBoxBorder EQU 0x5B41
DEF SetMapTextPointer EQU 0x3F2C
DEF TryLoadSaveFile EQU 0x75B9
DEF wUpdateSpritesEnabled EQU 0xCFD0
DEF DelayFrame EQU 0x20AB
DEF Delay3 EQU 0x3DF4
