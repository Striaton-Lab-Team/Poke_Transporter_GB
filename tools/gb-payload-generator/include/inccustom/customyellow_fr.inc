; ACE setup parameters

DEF ACE_ITEMID EQU 0x59
DEF ACE_POINTER EQU 0xDA64 ; use normal RAM equivalents

; NickWriter parameters

DEF wPayloadStart EQU 0xD8B9
DEF wNickWriter EQU 0xD66E
DEF AskName_Hijack EQU 0x631E
DEF ChangeCurrentBox EQU 0x7BE7
DEF PlaceStringAndDelay3 EQU 0x659B
