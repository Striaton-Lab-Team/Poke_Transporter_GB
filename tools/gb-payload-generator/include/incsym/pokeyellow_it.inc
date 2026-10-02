; ROM functions, highly variable between versions

DEF CopyData EQU 0xB1
DEF ClearVram EQU 0x1DC6
DEF GiveItem EQU 0x3E3D

DEF RemovePokemon EQU 0x3912
DEF JoypadLowSensitivity EQU 0x381C
DEF PrintBCDNumber EQU 0x13AD

; wRAM symbols, usually offset by constant amounts between versions/languages

DEF wTileMap EQU 0xC3A0
DEF wListScrollOffset EQU 0xCC36
DEF wBagSavedMenuItem EQU 0xCC2C
DEF wStringBuffer EQU 0xCF4F
DEF wPartyCount EQU 0xD167
DEF wMissableObjectFlags EQU 0xD5AA
DEF wNumBagItems EQU 0xD321
DEF wPokedexOwned EQU 0xD2FB
DEF wBoxCount EQU 0xDA84
DEF wDestinationWarpID EQU 0xD433
DEF wStatusFlags3 EQU 0xD731

; hRAM symbols, usually only vary between RB and Y

DEF hWarpDestinationMap EQU 0xFF8B
DEF hWY EQU 0xFFB0
DEF hJoy5 EQU 0xFFB5
DEF LoadCurrentBoxDataBank EQU 0x1C
DEF LoadCurrentBoxData EQU 0x7A3F
DEF SaveCurrentBoxData EQU 0x7B72
DEF Bankswitch EQU 0x3E82
DEF CallFunctionInTable EQU 0x3D91
DEF SoftReset EQU 0x1D08
DEF wCurrentMenuItem EQU 0xCC26
DEF wWhichPokemon EQU 0xCF96
DEF wRemoveMonFromBox EQU 0xCF99
DEF Serial_ExchangeBytes EQU 0x1FCB
DEF ClearScreen EQU 0x16DD
DEF PlaceString EQU 0x1723
DEF CableClub_TextBoxBorder EQU 0x5BB0
DEF SetMapTextPointer EQU 0x3F52
DEF TryLoadSaveFile EQU 0x7999
DEF wUpdateSpritesEnabled EQU 0xCFCF
DEF DelayFrame EQU 0x1E64
DEF Delay3 EQU 0x3DD9
