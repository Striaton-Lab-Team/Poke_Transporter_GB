; ROM functions, highly variable between versions

DEF CopyData EQU 0xB5
DEF ClearVram EQU 0x2004
DEF GiveItem EQU 0x3E48

DEF RemovePokemon EQU 0x3939
DEF JoypadLowSensitivity EQU 0x384B
DEF PrintBCDNumber EQU 0x15CD

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
DEF LoadCurrentBoxData EQU 0x766A
DEF SaveCurrentBoxData EQU 0x77BB
DEF Bankswitch EQU 0x35F0
DEF CallFunctionInTable EQU 0x3DB1
DEF SoftReset EQU 0x1F49
DEF wCurrentMenuItem EQU 0xCC26
DEF wWhichPokemon EQU 0xCF97
DEF wRemoveMonFromBox EQU 0xCF9A
DEF Serial_ExchangeBytes EQU 0x216F
DEF ClearScreen EQU 0x190F
DEF PlaceString EQU 0x1955
DEF CableClub_TextBoxBorder EQU 0x5ACD
DEF SetMapTextPointer EQU 0x3F29
DEF TryLoadSaveFile EQU 0x75C2
DEF wUpdateSpritesEnabled EQU 0xCFD0
DEF DelayFrame EQU 0x20AF
DEF Delay3 EQU 0x3DF1
