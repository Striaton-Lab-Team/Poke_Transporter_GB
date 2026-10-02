; bank $00
DEF NULL = $0000
DEF FarCall = $0008
DEF Bankswitch = $0010
DEF JumpTable = $0028
DEF SetUpTextbox EQU $1062
DEF CloseWindow EQU $1C00
DEF ReturnToMapFromSubmenu EQU $2214
DEF CallScript EQU $2609
DEF OpenSRAM EQU $2FB5
DEF CloseSRAM EQU $2FCB
DEF CopyBytes EQU $3010
DEF ByteFill EQU $302B
DEF JoyTextDelay_ForcehJoyDown EQU $3535
DEF PrintBCDNumber = $38A5

; bank $03
DEF EvoStoneEffect = $6de3
DEF EvoStoneEffectBank = $03
DEF CopyBoxmonToTempMon EQU $65bf
DEF CopyBoxmonToTempMonBank EQU $03
DEF RareCandyEffect = $6ee3
DEF RareCandyEffectBank = $03

; bank $04
DEF _ComposeMailMessage = $5e63
DEF _ComposeMailMessageBank = $04
DEF _NamingScreen = $56ae
DEF _NamingScreenBank = $04

; bank $05
DEF SaveChecksum EQU $4E13
DEF SaveChecksumBank EQU $05

; bank $09
DEF DecorationFlagAction_c = $6efc
DEF DecorationFlagAction_cBank = $09
DEF SelectQuantityToToss EQU $4FCB
DEF SelectQuantityToTossBank EQU $09

; bank $25

DEF DoPlayerEvent = $6c00
DEF DoPlayerEventBank = $25
DEF wScriptRunning = $d438

; bank $3f

DEF DebugColorPicker = $5b76
DEF DebugColorPickerBank = $3f

; bank $7f

DEF _GiveOddEgg = $74e0
DEF _GiveOddEggBank = $7f

; RAM

DEF wStackBottom EQU $C000
DEF ScreenTileStart EQU $C4A9
DEF ScreenTileCursor EQU $C550
DEF wOverworldMapBlocks EQU $C800
DEF wWildMonMoves EQU $C735
DEF wTileDown EQU $C2FA

DEF wMenuSelection EQU $CF74
DEF QuitFirstLetter EQU $CF8B
DEF wMenuCursorY EQU $CFA9

DEF wItemQuantity EQU $D10D
DEF wTempEnemyMonSpecies EQU $D204
DEF wEnemyMonMoves EQU $D208
DEF wEnemyMonDVs EQU $D20C
DEF wEnemyMonLevel EQU $D213
DEF wBattleMode EQU $D22D
DEF wPlayerState EQU $D95D
DEF wSpecialPhoneCallID EQU $DC31

DEF wItems = $d893
DEF wBalls = $d8d8
DEF wTilemap = $c4a0
DEF wTMsHMs = $d859

DEF wPartyCount = $dcd7
DEF wPartySpecies = $dcd8
DEF wPartyEnd = $dcde
DEF wPartyMon1Species = $dcdf
DEF wPartyMon1Item = $dce0
DEF wPartyMon1Moves = $dce1
DEF wPartyMon1ID = $dce5
DEF wPartyMon1Exp = $dce7
DEF wPartyMon1StatExp = $dcea
DEF wPartyMon1HPExp = $dcea
DEF wPartyMon1AtkExp = $dcec
DEF wPartyMon1DefExp = $dcee
DEF wPartyMon1SpdExp = $dcf0
DEF wPartyMon1SpcExp = $dcf2
DEF wPartyMon1DVs = $dcf4
DEF wPartyMon1PP = $dcf6
DEF wPartyMon1Happiness = $dcfa
DEF wPartyMon1PokerusStatus = $dcfb
DEF wPartyMon1CaughtData = $dcfc
DEF wPartyMon1Level = $dcfe
DEF wPartyMon1OT = $ddff
DEF wPartyMon1Nickname = $de41

DEF wCurItem = $d106
DEF wCurPartySpecies = $d108

DEF sPlayerData = $a009
DEF sPlayerDataBank = $01
DEF sBox = $ad10
DEF sBoxBank = $01
DEF sBoxMons = $ad26
DEF sBoxMonsBank = $01
DEF wLinkMode = $c2dc
DEF wOptions = $cfcc
DEF wPlayerID = $d47b
DEF wPlayerName = $d47d
DEF wRivalName = $d493
DEF wPlayerPalette = $d4dc
DEF wStatusFlags = $d84c
DEF wMoney = $d84e
DEF wCoins = $d855
DEF wBadges = $d857
DEF wNumKeyItems = $d8bc
DEF wEventFlags = $da72
DEF wVisitedSpawns = $dca5
DEF wPokedexCaught = $de99
DEF wPokedexSeen = $deb9
DEF wFirstUnownSeen = $def4

DEF sGSBallFlag = $be3c
DEF sGSBallFlagBank = $01
DEF sCrystalData = $be3d
DEF sCrystalDataBank = $01
DEF sGSBallFlagBackup = $be44
DEF sGSBallFlagBackupBank = $01
DEF wPlayerGender = $d472

; HRAM

DEF hTransferShadowOAM EQU $FF80
DEF hJoypadDown EQU $FFA4
DEF hJoyLast EQU $FFA9
DEF hBGMapMode EQU $FFD4
DEF DelayFrames EQU 0x0468
DEF PlaceString EQU 0x106E
DEF Predef EQU 0x2D6D
DEF RemoveMonFromPartyOrBox EQU 0x6038
DEF RemoveMonFromPartyOrBoxBank EQU 0x03
DEF SaveBox EQU 0x4E0C
DEF SaveBoxBank EQU 0x05
DEF LoadBox EQU 0x5021
DEF LoadBoxBank EQU 0x05
DEF wCurPartyMon EQU 0xD109
DEF wPokemonWithdrawDepositParameter EQU 0xD10B
DEF wCurBox EQU 0xDB72
DEF Reset EQU 0x0150
