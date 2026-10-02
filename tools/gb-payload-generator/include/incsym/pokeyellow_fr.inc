; ROM functions, highly variable between versions

DEF CopyData EQU 0xB1
DEF ClearVram EQU 0x1DC2
DEF GiveItem EQU 0x3E42

DEF RemovePokemon EQU 0x3917
DEF JoypadLowSensitivity EQU 0x3821
DEF PrintBCDNumber EQU 0x13AA

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
DEF LoadCurrentBoxData EQU 0x79CD
DEF SaveCurrentBoxData EQU 0x7B03
DEF Bankswitch EQU 0x3E87
DEF CallFunctionInTable EQU 0x3D96
DEF SoftReset EQU 0x1D01
DEF wCurrentMenuItem EQU 0xCC26
DEF wWhichPokemon EQU 0xCF96
DEF wRemoveMonFromBox EQU 0xCF99
DEF Serial_ExchangeBytes EQU 0x1FC7
DEF ClearScreen EQU 0x16DA
DEF PlaceString EQU 0x1720
DEF CableClub_TextBoxBorder EQU 0x5BE3
DEF SetMapTextPointer EQU 0x3F57
DEF TryLoadSaveFile EQU 0x792A
DEF wUpdateSpritesEnabled EQU 0xCFCF
DEF DelayFrame EQU 0x1E60
DEF Delay3 EQU 0x3DDE
