; ACE setup parameters

DEF ACE_ITEMID EQU 0x59
DEF ACE_POINTER EQU 0xDA64 ; use normal RAM equivalents

; NickWriter parameters

DEF wPayloadStart EQU 0xD8B4
DEF wNickWriter EQU 0xD669
DEF AskName_Hijack EQU 0x6295
DEF ChangeCurrentBox EQU 0x7C16
DEF PlaceStringAndDelay3 EQU 0x6512
