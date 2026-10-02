; bank $00
DEF NULL = $0000
DEF FarCall = $0008
DEF Bankswitch = $0010
DEF JumpTable = $0028
DEF SetUpTextbox EQU $0F3A
DEF CloseWindow EQU $1A61
DEF ReturnToMapFromSubmenu EQU $2025
DEF CallScript EQU $26F5
DEF OpenSRAM EQU $309D
DEF CloseSRAM EQU $30AD
DEF CopyBytes EQU $30D6
DEF ByteFill EQU $3108
DEF JoyTextDelay_ForcehJoyDown EQU $36F4
DEF PrintBCDNumber = $3339

; bank $03
DEF EvoStoneEffect = $71fc
DEF EvoStoneEffectBank = $03
DEF CopyBoxmonToTempMon EQU $68A0
DEF CopyBoxmonToTempMonBank EQU $03
DEF RareCandyEffect = $7318
DEF RareCandyEffectBank = $03

; bank $04
DEF _ComposeMailMessage = $60ae
DEF _ComposeMailMessageBank = $04
DEF _NamingScreen = $5a3d
DEF _NamingScreenBank = $04

; bank $05
DEF SaveChecksum EQU $4D8F
DEF SaveChecksumBank EQU $05

; bank $09
DEF DecorationFlagAction_c = $73d6
DEF DecorationFlagAction_cBank = $09
DEF SelectQuantityToToss EQU $4F26
DEF SelectQuantityToTossBank EQU $09

; bank $25

DEF DoPlayerEvent = $6b1e
DEF DoPlayerEventBank = $25
DEF wScriptRunning = $d151

; bank $3f
DEF DebugColorPicker = $54f1
DEF DebugColorPickerBank = $3f

; RAM

DEF ScreenTileStart EQU $C3A9
DEF ScreenTileCursor EQU $C450
DEF wOverworldMapBlocks EQU $C700
DEF wWildMonMoves EQU $CC07
DEF wTileDown EQU $CE97

DEF wMenuSelection EQU $CEDF
DEF QuitFirstLetter EQU $CEF6
DEF wMenuCursorY EQU $CF14

DEF wItemQuantity EQU $D004
DEF wTempEnemyMonSpecies EQU $D0DF
DEF wEnemyMonMoves EQU $D0E3
DEF wEnemyMonDVs EQU $D0E7
DEF wEnemyMonLevel EQU $D0EE
DEF wBattleMode EQU $D108
DEF wPlayerState EQU $D675
DEF NickWriter EQU $D6E5
DEF wSpecialPhoneCallID EQU $D941

DEF wItems = $d5ab
DEF wBalls = $d5f0
DEF wTilemap = $c3a0
DEF wTMsHMs = $d571

DEF wPartyCount = $d9eb
DEF wPartySpecies = $d9ec
DEF wPartyEnd = $d9ef
DEF wPartyMon1Species = $d9f0
DEF wPartyMon1Item = $d9f1
DEF wPartyMon1Moves = $d9f2
DEF wPartyMon1ID = $d9f6
DEF wPartyMon1Exp = $d9f8
DEF wPartyMon1StatExp = $d9fb
DEF wPartyMon1HPExp = $d9fb
DEF wPartyMon1AtkExp = $d9fd
DEF wPartyMon1DefExp = $d9ff
DEF wPartyMon1SpdExp = $da01
DEF wPartyMon1SpcExp = $da03
DEF wPartyMon1DVs = $da05
DEF wPartyMon1PP = $da07
DEF wPartyMon1Happiness = $da0b
DEF wPartyMon1PokerusStatus = $da0c
DEF wPartyMon1Unused1 = $da0d
DEF wPartyMon1Unused2 = $da0e
DEF wPartyMon1Level = $da0f
DEF wPartyMon1OT = $db10
DEF wPartyMon1Nickname = $db34

DEF wCurItem = $cffc
DEF wCurPartySpecies = $cffe

DEF sPlayerData = $a009
DEF sPlayerDataBank = $01
DEF sBox = $ad10
DEF sBoxBank = $01
DEF sBoxMons = $ad30
DEF sBoxMonsBank = $01
DEF wLinkMode = $d03c
DEF wOptions = $d1ab
DEF wPlayerID = $d1b3
DEF wPlayerName = $d1b5
DEF wRivalName = $d1c1
DEF wPlayerPalette = $d1f6
DEF wStatusFlags = $d564
DEF wMoney = $d566
DEF wCoins = $d56d
DEF wBadges = $d56f
DEF wNumKeyItems = $d5d4
DEF wEventFlags = $d7aa
DEF wVisitedSpawns = $d9b5
DEF wPokedexCaught = $db78
DEF wPokedexSeen = $db98
DEF wFirstUnownSeen = $dbd3

; HRAM

DEF hTransferShadowOAM EQU $FF80
DEF hJoypadDown EQU $FFA6
DEF hJoyLast EQU $FFAB
DEF hBGMapMode EQU $FFD6
DEF DelayFrames EQU 0x033C
DEF PlaceString EQU 0x0F46
DEF Predef EQU 0x2E05
DEF RemoveMonFromPartyOrBox EQU 0x62E2
DEF RemoveMonFromPartyOrBoxBank EQU 0x03
DEF SaveBox EQU 0x4D88
DEF SaveBoxBank EQU 0x05
DEF LoadBox EQU 0x4FD6
DEF LoadBoxBank EQU 0x05
DEF wCurPartyMon EQU 0xCFFF
DEF wPokemonWithdrawDepositParameter EQU 0xD002
DEF wCurBox EQU 0xD8AF
DEF Reset EQU 0x05AF
