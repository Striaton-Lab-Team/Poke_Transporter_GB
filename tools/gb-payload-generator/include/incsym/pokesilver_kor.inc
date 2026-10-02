; bank $00
DEF NULL = $0000
DEF FarCall = $0008
DEF Bankswitch = $0010
DEF JumpTable = $0028
DEF LoadStandardFont = $0d9c
DEF SetUpTextbox EQU $0F63
DEF CloseWindow EQU $1B07
DEF ReturnToMapFromSubmenu EQU $20D5
DEF CallScript EQU $27A5
DEF OpenSRAM EQU $317A
DEF CloseSRAM EQU $31BA
DEF CopyBytes EQU $31C2
DEF ByteFill EQU $31F4
DEF JoyTextDelay_ForcehJoyDown EQU $3779
DEF PrintBCDNumber EQU $3AC7

; bank $03
DEF EvoStoneEffect = $6e24
DEF EvoStoneEffectBank = $03
DEF CopyBoxmonToTempMon EQU $658C
DEF CopyBoxmonToTempMonBank EQU $03
DEF RareCandyEffect = $6f28
DEF RareCandyEffectBank = $03

; bank $04
DEF _NamingScreen = $5a32
DEF _NamingScreenBank = $04
DEF _ComposeMailMessage = $60d9
DEF _ComposeMailMessageBank = $04

; bank $05
DEF SaveChecksum EQU $4DFD
DEF SaveChecksumBank EQU $05

; bank $09
DEF DecorationFlagAction_c = $713a
DEF DecorationFlagAction_cBank = $09
DEF SelectQuantityToToss EQU $4F7F
DEF SelectQuantityToTossBank EQU $09

; bank $25

DEF DoPlayerEvent = $6b1e
DEF DoPlayerEventBank = $25
DEF wScriptRunning = $d21c

; bank $3f
DEF DebugColorPicker = $52c9
DEF DebugColorPickerBank = $3f 

; RAM

DEF ScreenTileStart EQU $C3A9
DEF ScreenTileCursor EQU $C450
DEF wOverworldMapBlocks EQU $C700
DEF wWildMonMoves EQU $CC1B
DEF wTileDown EQU $CE78

DEF wMenuSelection EQU $CE80
DEF QuitFirstLetter EQU $CE92
DEF wMenuCursorY EQU $CEB5

DEF wItemQuantity EQU $D0C5
DEF wTempEnemyMonSpecies EQU $D1AA
DEF wEnemyMonMoves EQU $D1AE
DEF wEnemyMonDVs EQU $D1B2
DEF wEnemyMonLevel EQU $D1B9
DEF wBattleMode EQU $D1D3
DEF wPlayerState EQU $D735
DEF NickWriter EQU $D785
DEF wSpecialPhoneCallID EQU $DA7E

DEF wItems = $d66b
DEF wBalls = $d6b0
DEF wTilemap = $c3a0
DEF wTMsHMs = $d631

DEF wPartyCount = $db1f
DEF wPartySpecies = $db20
DEF wPartyEnd = $db26
DEF wPartyMon1Species = $db27
DEF wPartyMon1Item = $db28
DEF wPartyMon1Moves = $db29
DEF wPartyMon1ID = $db2d
DEF wPartyMon1Exp = $db2f
DEF wPartyMon1StatExp = $db32
DEF wPartyMon1HPExp = $db32
DEF wPartyMon1AtkExp = $db34
DEF wPartyMon1DefExp = $db36
DEF wPartyMon1SpdExp = $db38
DEF wPartyMon1SpcExp = $db3a
DEF wPartyMon1DVs = $db3c
DEF wPartyMon1PP = $db3e
DEF wPartyMon1Happiness = $db42
DEF wPartyMon1PokerusStatus = $db43
DEF wPartyMon1Unused1 = $db44
DEF wPartyMon1Unused2 = $db45
DEF wPartyMon1Level = $db46
DEF wPartyMon1OT = $dc47
DEF wPartyMon1Nickname = $dc89

DEF wCurItem = $d0be
DEF wCurPartySpecies = $d0c0

DEF sPlayerData = $a009
DEF sPlayerDataBank = $01
DEF sBox = $adae
DEF sBoxBank = $01
DEF sBoxMons = $adc4
DEF sBoxMonsBank = $01
DEF wLinkMode = $d0fd
DEF wOptions = $d254
DEF wPlayerID = $d25c
DEF wPlayerName = $d25e
DEF wRivalName = $d269
DEF wPlayerPalette = $d2b8
DEF wStatusFlags = $d624
DEF wMoney = $d626
DEF wCoins = $d62d
DEF wBadges = $d62f
DEF wNumKeyItems = $d694
DEF wEventFlags = $d84a
DEF wVisitedSpawns = $daeb
DEF wPokedexCaught = $dce1
DEF wPokedexSeen = $dd01
DEF wFirstUnownSeen = $dd3c

; HRAM

DEF hTransferShadowOAM EQU $FF80
DEF hJoypadDown EQU $FFA6
DEF hJoyLast EQU $FFAB
DEF hBGMapMode EQU $FFD6
DEF DelayFrames EQU 0x033C
DEF PlaceString EQU 0x0F6F
DEF Predef EQU 0x2ED0
DEF RemoveMonFromPartyOrBox EQU 0x6013
DEF RemoveMonFromPartyOrBoxBank EQU 0x03
DEF SaveBox EQU 0x4DF6
DEF SaveBoxBank EQU 0x05
DEF LoadBox EQU 0x5065
DEF LoadBoxBank EQU 0x05
DEF wCurPartyMon EQU 0xD0C1
DEF wPokemonWithdrawDepositParameter EQU 0xD0C3
DEF wCurBox EQU 0xD94F
DEF Reset EQU 0x05B4
