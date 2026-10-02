; bank $00
DEF NULL = $0000
DEF FarCall = $0008
DEF Bankswitch = $0010
DEF JumpTable = $0028
DEF SetUpTextbox EQU $104B
DEF CloseWindow EQU $1BE2
DEF ReturnToMapFromSubmenu EQU $21ED
DEF CallScript EQU $25E2
DEF OpenSRAM EQU $2F9D
DEF CloseSRAM EQU $2FAD
DEF CopyBytes EQU $2FF2
DEF ByteFill EQU $300D
DEF JoyTextDelay_ForcehJoyDown EQU $351B
DEF PrintBCDNumber = $388B

; bank $03
DEF EvoStoneEffect = $7146
DEF EvoStoneEffectBank = $03
DEF CopyBoxmonToTempMon EQU $6878
DEF CopyBoxmonToTempMonBank EQU $03
DEF RareCandyEffect = $7262
DEF RareCandyEffectBank = $03

; bank $04
DEF _ComposeMailMessage = $5ceb
DEF _ComposeMailMessageBank = $04
DEF _NamingScreen = $5651
DEF _NamingScreenBank = $04

; bank $05
DEF SaveChecksum EQU $4DF5
DEF SaveChecksumBank EQU $05

; bank $09
DEF DecorationFlagAction_c = $71ff
DEF DecorationFlagAction_cBank = $09
DEF SelectQuantityToToss EQU $4FCB
DEF SelectQuantityToTossBank EQU $09

; bank $25

DEF DoPlayerEvent = $6beb
DEF DoPlayerEventBank = $25
DEF wScriptRunning = $d429

; bank $3f

DEF DebugColorPicker = $53e6
DEF DebugColorPickerBank = $3f

; RAM

DEF wStackBottom EQU $C000
DEF ScreenTileStart EQU $C4A9
DEF ScreenTileCursor EQU $C550
DEF wOverworldMapBlocks EQU $C800
DEF wWildMonMoves EQU $C729
DEF wTileDown EQU $D13E

DEF wMenuSelection EQU $CF68
DEF QuitFirstLetter EQU $CF7F
DEF wMenuCursorY EQU $CF9D

DEF wItemQuantity EQU $D0CD
DEF wTempEnemyMonSpecies EQU $D235
DEF wEnemyMonMoves EQU $D239
DEF wEnemyMonDVs EQU $D23D
DEF wEnemyMonLevel EQU $D244
DEF wBattleMode EQU $D25E
DEF wPlayerState EQU $D950
DEF wSpecialPhoneCallID EQU $DBF7

DEF wItems = $d886
DEF wBalls = $d8cb
DEF wTilemap = $c4a0
DEF wTMsHMs = $d84c

DEF wPartyCount = $dc9d
DEF wPartySpecies = $dc9e
DEF wPartyEnd = $dca4
DEF wPartyMon1Species = $dca5
DEF wPartyMon1Item = $dca6
DEF wPartyMon1Moves = $dca7
DEF wPartyMon1ID = $dcab
DEF wPartyMon1Exp = $dcad
DEF wPartyMon1StatExp = $dcb0
DEF wPartyMon1HPExp = $dcb0
DEF wPartyMon1AtkExp = $dcb2
DEF wPartyMon1DefExp = $dcb4
DEF wPartyMon1SpdExp = $dcb6
DEF wPartyMon1SpcExp = $dcb8
DEF wPartyMon1DVs = $dcba
DEF wPartyMon1PP = $dcbc
DEF wPartyMon1Happiness = $dcc0
DEF wPartyMon1PokerusStatus = $dcc1
DEF wPartyMon1CaughtData = $dcc2
DEF wPartyMon1Level = $dcc4
DEF wPartyMon1OT = $ddc5
DEF wPartyMon1Nickname = $dde9

DEF wCurItem = $d0c6
DEF wCurPartySpecies = $d0c8

DEF sPlayerData = $a009
DEF sPlayerDataBank = $01
DEF sBox = $ad10
DEF sBoxBank = $01
DEF sBoxMons = $ad30
DEF sBoxMonsBank = $01
DEF wLinkMode = $c2dc
DEF wOptions = $cfc0
DEF wPlayerID = $d48c
DEF wPlayerName = $d48e
DEF wRivalName = $d49a
DEF wPlayerPalette = $d4cf
DEF wStatusFlags = $d83f
DEF wMoney = $d844
DEF wCoins = $d848
DEF wBadges = $d84a
DEF wNumKeyItems = $d8af
DEF wEventFlags = $da65
DEF wVisitedSpawns = $dc6c
DEF wPokedexCaught = $de2d
DEF wPokedexSeen = $de4d
DEF wFirstUnownSeen = $de88

DEF sGSBallFlag = $a000
DEF sGSBallFlagBank = $05
DEF sCrystalData = $be3d
DEF sCrystalDataBank = $04
DEF sGSBallFlagBackup = $a083
DEF sGSBallFlagBackupBank = $05
DEF wPlayerGender = $d463

; HRAM

DEF hTransferShadowOAM EQU $FF80
DEF hJoypadDown EQU $FFA4
DEF hJoyLast EQU $FFA9
DEF hBGMapMode EQU $FFD4
DEF DelayFrames EQU 0x0468
DEF PlaceString EQU 0x1057
DEF Predef EQU 0x2D55
DEF RemoveMonFromPartyOrBox EQU 0x628F
DEF RemoveMonFromPartyOrBoxBank EQU 0x03
DEF SaveBox EQU 0x4DEE
DEF SaveBoxBank EQU 0x05
DEF LoadBox EQU 0x4FEF
DEF LoadBoxBank EQU 0x05
DEF wCurPartyMon EQU 0xD0C9
DEF wPokemonWithdrawDepositParameter EQU 0xD0CB
DEF wCurBox EQU 0xDB65
DEF Reset EQU 0x0150
