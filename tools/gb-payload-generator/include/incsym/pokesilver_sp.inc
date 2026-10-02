; bank $00
DEF NULL = $0000
DEF FarCall = $0008
DEF Bankswitch = $0010
DEF JumpTable = $0028
DEF SetUpTextbox EQU $0F68
DEF CloseWindow EQU $1AD1
DEF ReturnToMapFromSubmenu EQU $209C
DEF CallScript EQU $276D
DEF OpenSRAM EQU $3105
DEF CloseSRAM EQU $3115
DEF CopyBytes EQU $313E
DEF ByteFill EQU $3170
DEF JoyTextDelay_ForcehJoyDown EQU $374D
DEF PrintBCDNumber = $3A9B

; bank $03
DEF EvoStoneEffect = $6e31
DEF EvoStoneEffectBank = $03
DEF CopyBoxmonToTempMon EQU $6594
DEF CopyBoxmonToTempMonBank EQU $03
DEF RareCandyEffect = $6f39
DEF RareCandyEffectBank = $03

; bank $04
DEF _ComposeMailMessage = $6299
DEF _ComposeMailMessageBank = $04
DEF _NamingScreen = $5ab6
DEF _NamingScreenBank = $04

; bank $05
DEF SaveChecksum EQU $4E0C
DEF SaveChecksumBank EQU $05

; bank $09
DEF DecorationFlagAction_c = $70d9
DEF DecorationFlagAction_cBank = $09
DEF SelectQuantityToToss EQU $4F22
DEF SelectQuantityToTossBank EQU $09

; bank $25

DEF DoPlayerEvent = $6b1e
DEF DoPlayerEventBank = $25
DEF wScriptRunning = $d15f

; bank $3f
DEF DebugColorPicker = $52c9
DEF DebugColorPickerBank = $3f 

; RAM

DEF ScreenTileStart EQU $C3A9
DEF ScreenTileCursor EQU $C450
DEF wOverworldMapBlocks EQU $C700
DEF wWildMonMoves EQU $CC13
DEF wTileDown EQU $CEA3

DEF wMenuSelection EQU $CEAB
DEF QuitFirstLetter EQU $CEC2
DEF wMenuCursorY EQU $CEE0

DEF wItemQuantity EQU $D00A
DEF wTempEnemyMonSpecies EQU $D0ED
DEF wEnemyMonMoves EQU $D0F1
DEF wEnemyMonDVs EQU $D0F5
DEF wEnemyMonLevel EQU $D0FC
DEF wBattleMode EQU $D116
DEF wPlayerState EQU $D682
DEF NickWriter EQU $D6F2
DEF wSpecialPhoneCallID EQU $D97B

DEF wItems = $d5b8
DEF wBalls = $d5fd
DEF wTilemap = $c3a0
DEF wTMsHMs = $d57e

DEF wPartyCount = $da22
DEF wPartySpecies = $da23
DEF wPartyEnd = $da29
DEF wPartyMon1Species = $da2a
DEF wPartyMon1Item = $da2b
DEF wPartyMon1Moves = $da2c
DEF wPartyMon1ID = $da30
DEF wPartyMon1Exp = $da32
DEF wPartyMon1StatExp = $da35
DEF wPartyMon1HPExp = $da35
DEF wPartyMon1AtkExp = $da37
DEF wPartyMon1DefExp = $da39
DEF wPartyMon1SpdExp = $da3b
DEF wPartyMon1SpcExp = $da3d
DEF wPartyMon1DVs = $da3f
DEF wPartyMon1PP = $da41
DEF wPartyMon1Happiness = $da45
DEF wPartyMon1PokerusStatus = $da46
DEF wPartyMon1Unused1 = $da47
DEF wPartyMon1Unused2 = $da48
DEF wPartyMon1Level = $da49
DEF wPartyMon1OT = $db4a
DEF wPartyMon1Nickname = $db8c

DEF wCurItem = $d002
DEF wCurPartySpecies = $d004

DEF sPlayerData = $a009
DEF sPlayerDataBank = $01
DEF sBox = $ad6c
DEF sBoxBank = $01
DEF sBoxMons = $ad82
DEF sBoxMonsBank = $01
DEF wLinkMode = $d042
DEF wOptions = $d199
DEF wPlayerID = $d1a1
DEF wPlayerName = $d1a3
DEF wRivalName = $d1b9
DEF wPlayerPalette = $d203
DEF wStatusFlags = $d571
DEF wMoney = $d573
DEF wCoins = $d57a
DEF wBadges = $d57c
DEF wNumKeyItems = $d5e1
DEF wEventFlags = $d7b7
DEF wVisitedSpawns = $d9ee
DEF wPokedexCaught = $dbe4
DEF wPokedexSeen = $dc04
DEF wFirstUnownSeen = $dc3f

; HRAM

DEF hTransferShadowOAM EQU $FF80
DEF hJoypadDown EQU $FFA6
DEF hJoyLast EQU $FFAB
DEF hBGMapMode EQU $FFD6
DEF DelayFrames EQU 0x033C
DEF PlaceString EQU 0x0F74
DEF Predef EQU 0x2E6D
DEF RemoveMonFromPartyOrBox EQU 0x603E
DEF RemoveMonFromPartyOrBoxBank EQU 0x03
DEF SaveBox EQU 0x4E05
DEF SaveBoxBank EQU 0x05
DEF LoadBox EQU 0x507A
DEF LoadBoxBank EQU 0x05
DEF wCurPartyMon EQU 0xD005
DEF wPokemonWithdrawDepositParameter EQU 0xD008
DEF wCurBox EQU 0xD8BC
DEF Reset EQU 0x05B0
