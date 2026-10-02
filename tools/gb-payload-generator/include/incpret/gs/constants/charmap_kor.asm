; $00-$16 are TX_* constants (see macros/scripts/text.asm)

; Control characters (see home/text.asm)

	charmap "<NULL>",    $00
	charmap "<CR>",      $16
	charmap "<BSP>",     $1f ; breakable space (usually " ", or "<LF>" on the Town Map)
	charmap "<LF>",      $22
	charmap "<POKE>",    $24 ; "<PO><KE>"
	charmap "<WBR>",     $25 ; word-break opportunity (usually skipped, or "<LF>" on the Town Map)
	charmap "<RED>",     $38 ; wRedsName
	charmap "<GREEN>",   $39 ; wGreensName
	charmap "<ENEMY>",   $3f
	charmap "<MOM>",     $49 ; wMomsName
	charmap "<PKMN>",    $4a ; "<PK><MN>"
	charmap "<_CONT>",   $4b ; implements "<CONT>"
	charmap "<SCROLL>",  $4c
	charmap "<NEXT>",    $4e
	charmap "<LINE>",    $4f
	charmap "@",         $50 ; string terminator
	charmap "<PARA>",    $51
	charmap "<PLAYER>",  $52 ; wPlayerName
	charmap "<RIVAL>",   $53 ; wRivalName
	charmap "#",         $54 ; "POKé"
	charmap "<CONT>",    $55
	charmap "<……>",      $56 ; "……"
	charmap "<DONE>",    $57
	charmap "<PROMPT>",  $58
	charmap "<TARGET>",  $59
	charmap "<USER>",    $5a
	charmap "<PC>",      $5b ; "PC"
	charmap "<TM>",      $5c ; "TM"
	charmap "<TRAINER>", $5d ; "TRAINER"
	charmap "<ROCKET>",  $5e ; "ROCKET"
	charmap "<DEXEND>",  $5f

; Actual characters (from gfx/font/font_extra.png)

	charmap "<BOLD_A>",  $60 ; unused
	charmap "<BOLD_B>",  $61 ; unused
	charmap "<BOLD_C>",  $62 ; unused
	charmap "<BOLD_D>",  $63 ; unused
	charmap "<BOLD_E>",  $64 ; unused
	charmap "<BOLD_F>",  $65 ; unused
	charmap "<BOLD_G>",  $66 ; unused
	charmap "<BOLD_H>",  $67 ; unused
	charmap "<BOLD_I>",  $68 ; unused
	charmap "<BOLD_V>",  $69
	charmap "<BOLD_S>",  $6a
	charmap "<BOLD_L>",  $6b ; unused
	charmap "<BOLD_M>",  $6c ; unused
	charmap "<COLON>",   $6d ; colon with tinier dots than ":"
	charmap "ぃ",         $6e ; hiragana small i, unused
	charmap "ぅ",         $6f ; hiragana small u, unused
	charmap "<PO>",      $70
	charmap "<KE>",      $71
	charmap "“",         $72 ; opening quote
	charmap "”",         $73 ; closing quote
	charmap "·",         $74 ; middle dot, unused
	charmap "…",         $75 ; ellipsis
	charmap "ぁ",         $76 ; hiragana small a, unused
	charmap "ぇ",         $77 ; hiragana small e, unused
	charmap "ぉ",         $78 ; hiragana small o, unused

	charmap "┌",         $79
	charmap "─",         $7a
	charmap "┐",         $7b
	charmap "│",         $7c
	charmap "└",         $7d
	charmap "┘",         $7e
	charmap " ",         $7f

; Actual characters (from gfx/font/font_battle_extra.png)

	charmap "<LV>",      $6e

	charmap "<DO>",      $70 ; hiragana small do, unused
	charmap "◀",         $71
	charmap "『",         $72 ; Japanese opening quote, unused
	charmap "<ID>",      $73
	charmap "№",         $74

; Actual characters (from other graphics files)

	; needed for _LoadFontsExtra1 (see engine/gfx/load_font.asm)
	charmap "■",         $60 ; gfx/font/black.2bpp
	charmap "▲",         $61 ; gfx/font/up_arrow.png
	charmap "☎",         $62 ; gfx/font/phone_icon.2bpp

	; needed for MagikarpHouseSign (see engine/events/magikarp.asm)
	charmap "′",         $6e ; gfx/font/feet_inches.png
	charmap "″",         $6f ; gfx/font/feet_inches.png

	; needed for StatsScreen_PlaceShinyIcon and PrintPartyMonPage1
	charmap "⁂",         $3f ; gfx/stats/stats_tiles.png, tile 14

; Actual Hangul characters

	charmap "가",		$01, $01
	charmap "각",		$01, $02
	charmap "간",		$01, $03
	charmap "갇",		$01, $04
	charmap "갈",		$01, $05
	charmap "갉",		$01, $06
	charmap "갊",		$01, $07
	charmap "감",		$01, $08
	charmap "갑",		$01, $09
	charmap "값",		$01, $0a
	charmap "갓",		$01, $0b
	charmap "갔",		$01, $0c
	charmap "강",		$01, $0d
	charmap "갖",		$01, $0e
	charmap "갗",		$01, $0f
	charmap "같",		$01, $10
	charmap "갚",		$01, $11
	charmap "갛",		$01, $12
	charmap "개",		$01, $13
	charmap "객",		$01, $14
	charmap "갠",		$01, $15
	charmap "갤",		$01, $16
	charmap "갬",		$01, $17
	charmap "갭",		$01, $18
	charmap "갯",		$01, $19
	charmap "갰",		$01, $1a
	charmap "갱",		$01, $1b
	charmap "갸",		$01, $1c
	charmap "갹",		$01, $1d
	charmap "갼",		$01, $1e
	charmap "걀",		$01, $1f
	charmap "걋",		$01, $20
	charmap "걍",		$01, $21
	charmap "걔",		$01, $22
	charmap "걘",		$01, $23
	charmap "걜",		$01, $24
	charmap "거",		$01, $25
	charmap "걱",		$01, $26
	charmap "건",		$01, $27
	charmap "걷",		$01, $28
	charmap "걸",		$01, $29
	charmap "걺",		$01, $2a
	charmap "검",		$01, $2b
	charmap "겁",		$01, $2c
	charmap "것",		$01, $2d
	charmap "겄",		$01, $2e
	charmap "겅",		$01, $2f
	charmap "겆",		$01, $30
	charmap "겉",		$01, $31
	charmap "겊",		$01, $32
	charmap "겋",		$01, $33
	charmap "게",		$01, $34
	charmap "겐",		$01, $35
	charmap "겔",		$01, $36
	charmap "겜",		$01, $37
	charmap "겝",		$01, $38
	charmap "겟",		$01, $39
	charmap "겠",		$01, $3a
	charmap "겡",		$01, $3b
	charmap "겨",		$01, $3c
	charmap "격",		$01, $3d
	charmap "겪",		$01, $3e
	charmap "견",		$01, $3f
	charmap "겯",		$01, $40
	charmap "결",		$01, $41
	charmap "겹",		$01, $42
	charmap "겸",		$01, $43
	charmap "겻",		$01, $44
	charmap "겼",		$01, $45
	charmap "경",		$01, $46
	charmap "곁",		$01, $47
	charmap "계",		$01, $48
	charmap "곈",		$01, $49
	charmap "곌",		$01, $4a
	charmap "곕",		$01, $4b
	charmap "곗",		$01, $4c
	charmap "고",		$01, $4d
	charmap "곡",		$01, $4e
	charmap "곤",		$01, $4f
	charmap "곧",		$01, $60
	charmap "골",		$01, $61
	charmap "곪",		$01, $62
	charmap "곬",		$01, $63
	charmap "곯",		$01, $64
	charmap "곰",		$01, $65
	charmap "곱",		$01, $66
	charmap "곳",		$01, $67
	charmap "공",		$01, $68
	charmap "곶",		$01, $69
	charmap "과",		$01, $6a
	charmap "곽",		$01, $6b
	charmap "관",		$01, $6c
	charmap "괄",		$01, $6d
	charmap "괆",		$01, $6e
	charmap "괌",		$01, $71
	charmap "괍",		$01, $72
	charmap "괏",		$01, $73
	charmap "광",		$01, $74
	charmap "괘",		$01, $75
	charmap "괜",		$01, $76
	charmap "괠",		$01, $77
	charmap "괩",		$01, $78
	charmap "괬",		$01, $79
	charmap "괭",		$01, $7a
	charmap "괴",		$01, $7b
	charmap "괵",		$01, $7c
	charmap "괸",		$01, $7d
	charmap "괼",		$01, $7e
	charmap "괻",		$01, $7f
	charmap "굅",		$01, $80
	charmap "굇",		$01, $81
	charmap "굉",		$01, $82
	charmap "교",		$01, $83
	charmap "굔",		$01, $84
	charmap "굘",		$01, $85
	charmap "굡",		$01, $86
	charmap "굣",		$01, $87
	charmap "구",		$01, $88
	charmap "국",		$01, $89
	charmap "군",		$01, $8a
	charmap "굳",		$01, $8b
	charmap "굴",		$01, $8c
	charmap "굵",		$01, $8d
	charmap "굶",		$01, $8e
	charmap "굻",		$01, $8f
	charmap "굼",		$01, $90
	charmap "굽",		$01, $91
	charmap "굿",		$01, $92
	charmap "궁",		$01, $93
	charmap "궂",		$01, $94
	charmap "궈",		$01, $95
	charmap "궉",		$01, $96
	charmap "권",		$01, $97
	charmap "궐",		$01, $98
	charmap "궜",		$01, $99
	charmap "궝",		$01, $9a
	charmap "궤",		$01, $9b
	charmap "궷",		$01, $9c
	charmap "귀",		$01, $9d
	charmap "귁",		$01, $9e
	charmap "귄",		$01, $9f
	charmap "귈",		$01, $a0
	charmap "귐",		$01, $a1
	charmap "귑",		$01, $a2
	charmap "귓",		$01, $a3
	charmap "규",		$01, $a4
	charmap "균",		$01, $a5
	charmap "귤",		$01, $a6
	charmap "그",		$01, $a7
	charmap "극",		$01, $a8
	charmap "근",		$01, $a9
	charmap "귿",		$01, $aa
	charmap "글",		$01, $ab
	charmap "긁",		$01, $ac
	charmap "금",		$01, $ad
	charmap "급",		$01, $ae
	charmap "긋",		$01, $af
	charmap "긍",		$01, $b0
	charmap "긔",		$01, $b1
	charmap "기",		$01, $b2
	charmap "긱",		$01, $b3
	charmap "긴",		$01, $b4
	charmap "긷",		$01, $b5
	charmap "길",		$01, $b6
	charmap "긺",		$01, $b7
	charmap "김",		$01, $b8
	charmap "깁",		$01, $b9
	charmap "깃",		$01, $ba
	charmap "깅",		$01, $bb
	charmap "깆",		$01, $bc
	charmap "깊",		$01, $bd
	charmap "까",		$01, $be
	charmap "깍",		$01, $bf
	charmap "깎",		$01, $c0
	charmap "깐",		$01, $c1
	charmap "깔",		$01, $c2
	charmap "깖",		$01, $c3
	charmap "깜",		$01, $c4
	charmap "깝",		$01, $c5
	charmap "깟",		$01, $c6
	charmap "깠",		$01, $c7
	charmap "깡",		$01, $c8
	charmap "깥",		$01, $c9
	charmap "깨",		$01, $ca
	charmap "깩",		$01, $cb
	charmap "깬",		$01, $cc
	charmap "깰",		$01, $cd
	charmap "깸",		$01, $ce
	charmap "깹",		$01, $d1
	charmap "깻",		$01, $d2
	charmap "깼",		$01, $d3
	charmap "깽",		$01, $d4
	charmap "꺄",		$01, $d5
	charmap "꺅",		$01, $d6
	charmap "꺌",		$01, $d7
	charmap "꺼",		$01, $d8
	charmap "꺽",		$01, $d9
	charmap "꺾",		$01, $da
	charmap "껀",		$01, $db
	charmap "껄",		$01, $dc
	charmap "껌",		$01, $dd
	charmap "껍",		$01, $de
	charmap "껏",		$01, $df
	charmap "껐",		$01, $e0
	charmap "껑",		$01, $e1
	charmap "께",		$01, $e2
	charmap "껙",		$01, $e3
	charmap "껜",		$01, $e4
	charmap "껨",		$01, $e5
	charmap "껫",		$01, $e6
	charmap "껭",		$01, $e7
	charmap "껴",		$01, $e8
	charmap "껸",		$01, $e9
	charmap "껼",		$01, $ea
	charmap "꼇",		$01, $eb
	charmap "꼈",		$01, $ec
	charmap "꼍",		$01, $ed
	charmap "꼐",		$01, $ee
	charmap "꼬",		$01, $ef
	charmap "꼭",		$01, $f0
	charmap "꼰",		$01, $f1
	charmap "꼲",		$01, $f2
	charmap "꼴",		$01, $f3
	charmap "꼼",		$01, $f4
	charmap "꼽",		$01, $f5
	charmap "꼿",		$01, $f6
	charmap "꽁",		$01, $f7
	charmap "꽂",		$01, $f8
	charmap "꽃",		$01, $f9
	charmap "꽈",		$01, $fa
	charmap "꽉",		$01, $fb
	charmap "꽐",		$01, $fc
	charmap "꽜",		$01, $fd
	charmap "꽝",		$01, $fe
	charmap "꽤",		$01, $ff

	charmap "꽥",		$02, $00
	charmap "꽹",		$02, $01
	charmap "꾀",		$02, $02
	charmap "꾄",		$02, $03
	charmap "꾈",		$02, $04
	charmap "꾐",		$02, $05
	charmap "꾑",		$02, $06
	charmap "꾕",		$02, $07
	charmap "꾜",		$02, $08
	charmap "꾸",		$02, $09
	charmap "꾹",		$02, $0a
	charmap "꾼",		$02, $0b
	charmap "꿀",		$02, $0c
	charmap "꿇",		$02, $0d
	charmap "꿈",		$02, $0e
	charmap "꿉",		$02, $0f
	charmap "꿋",		$02, $10
	charmap "꿍",		$02, $11
	charmap "꿎",		$02, $12
	charmap "꿔",		$02, $13
	charmap "꿜",		$02, $14
	charmap "꿨",		$02, $15
	charmap "꿩",		$02, $16
	charmap "꿰",		$02, $17
	charmap "꿱",		$02, $18
	charmap "꿴",		$02, $19
	charmap "꿸",		$02, $1a
	charmap "뀀",		$02, $1b
	charmap "뀁",		$02, $1c
	charmap "뀄",		$02, $1d
	charmap "뀌",		$02, $1e
	charmap "뀐",		$02, $1f
	charmap "뀔",		$02, $20
	charmap "뀜",		$02, $21
	charmap "뀝",		$02, $22
	charmap "뀨",		$02, $23
	charmap "끄",		$02, $24
	charmap "끅",		$02, $25
	charmap "끈",		$02, $26
	charmap "끊",		$02, $27
	charmap "끌",		$02, $28
	charmap "끎",		$02, $29
	charmap "끓",		$02, $2a
	charmap "끔",		$02, $2b
	charmap "끕",		$02, $2c
	charmap "끗",		$02, $2d
	charmap "끙",		$02, $2e
	charmap "끝",		$02, $31
	charmap "끼",		$02, $32
	charmap "끽",		$02, $33
	charmap "낀",		$02, $34
	charmap "낄",		$02, $35
	charmap "낌",		$02, $36
	charmap "낍",		$02, $37
	charmap "낏",		$02, $38
	charmap "낑",		$02, $39
	charmap "나",		$02, $3a
	charmap "낙",		$02, $3b
	charmap "낚",		$02, $3c
	charmap "난",		$02, $3d
	charmap "낟",		$02, $3e
	charmap "날",		$02, $3f
	charmap "낡",		$02, $40
	charmap "낢",		$02, $41
	charmap "남",		$02, $42
	charmap "납",		$02, $43
	charmap "낫",		$02, $44
	charmap "났",		$02, $45
	charmap "낭",		$02, $46
	charmap "낮",		$02, $47
	charmap "낯",		$02, $48
	charmap "낱",		$02, $49
	charmap "낳",		$02, $4a
	charmap "내",		$02, $4b
	charmap "낵",		$02, $4c
	charmap "낸",		$02, $4d
	charmap "낼",		$02, $4e
	charmap "냄",		$02, $4f
	charmap "냅",		$02, $60
	charmap "냇",		$02, $61
	charmap "냈",		$02, $62
	charmap "냉",		$02, $63
	charmap "냐",		$02, $64
	charmap "냑",		$02, $65
	charmap "냔",		$02, $66
	charmap "냘",		$02, $67
	charmap "냠",		$02, $68
	charmap "냥",		$02, $69
	charmap "너",		$02, $6a
	charmap "넉",		$02, $6b
	charmap "넋",		$02, $6c
	charmap "넌",		$02, $6d
	charmap "널",		$02, $6e
	charmap "넒",		$02, $6f
	charmap "넓",		$02, $70
	charmap "넘",		$02, $71
	charmap "넙",		$02, $72
	charmap "넛",		$02, $73
	charmap "넜",		$02, $74
	charmap "넝",		$02, $75
	charmap "넣",		$02, $76
	charmap "네",		$02, $77
	charmap "넥",		$02, $78
	charmap "넨",		$02, $79
	charmap "넬",		$02, $7a
	charmap "넴",		$02, $7b
	charmap "넵",		$02, $7c
	charmap "넷",		$02, $7d
	charmap "넸",		$02, $7e
	charmap "넹",		$02, $7f
	charmap "녀",		$02, $80
	charmap "녁",		$02, $81
	charmap "년",		$02, $82
	charmap "녈",		$02, $83
	charmap "념",		$02, $84
	charmap "녑",		$02, $85
	charmap "녔",		$02, $86
	charmap "녕",		$02, $87
	charmap "녘",		$02, $88
	charmap "녜",		$02, $89
	charmap "녠",		$02, $8a
	charmap "노",		$02, $8b
	charmap "녹",		$02, $8c
	charmap "논",		$02, $8d
	charmap "놀",		$02, $8e
	charmap "놂",		$02, $8f
	charmap "놈",		$02, $90
	charmap "놉",		$02, $91
	charmap "놋",		$02, $92
	charmap "농",		$02, $93
	charmap "높",		$02, $94
	charmap "놓",		$02, $95
	charmap "놔",		$02, $96
	charmap "놘",		$02, $97
	charmap "놜",		$02, $98
	charmap "놨",		$02, $99
	charmap "뇌",		$02, $9a
	charmap "뇐",		$02, $9b
	charmap "뇔",		$02, $9c
	charmap "뇜",		$02, $9d
	charmap "뇝",		$02, $9e
	charmap "뇟",		$02, $a1
	charmap "뇨",		$02, $a2
	charmap "뇩",		$02, $a3
	charmap "뇬",		$02, $a4
	charmap "뇰",		$02, $a5
	charmap "뇹",		$02, $a6
	charmap "뇻",		$02, $a7
	charmap "뇽",		$02, $a8
	charmap "누",		$02, $a9
	charmap "눅",		$02, $aa
	charmap "눈",		$02, $ab
	charmap "눋",		$02, $ac
	charmap "눌",		$02, $ad
	charmap "눔",		$02, $ae
	charmap "눕",		$02, $af
	charmap "눗",		$02, $b0
	charmap "눙",		$02, $b1
	charmap "눠",		$02, $b2
	charmap "눴",		$02, $b3
	charmap "눼",		$02, $b4
	charmap "뉘",		$02, $b5
	charmap "뉜",		$02, $b6
	charmap "뉠",		$02, $b7
	charmap "뉨",		$02, $b8
	charmap "뉩",		$02, $b9
	charmap "뉴",		$02, $ba
	charmap "뉵",		$02, $bb
	charmap "뉼",		$02, $bc
	charmap "늄",		$02, $bd
	charmap "늅",		$02, $be
	charmap "늉",		$02, $bf
	charmap "느",		$02, $c0
	charmap "늑",		$02, $c1
	charmap "는",		$02, $c2
	charmap "늘",		$02, $c3
	charmap "늙",		$02, $c4
	charmap "늚",		$02, $c5
	charmap "늠",		$02, $c6
	charmap "늡",		$02, $c7
	charmap "늣",		$02, $c8
	charmap "능",		$02, $c9
	charmap "늦",		$02, $ca
	charmap "늪",		$02, $cb
	charmap "늬",		$02, $cc
	charmap "늰",		$02, $cd
	charmap "늴",		$02, $ce
	charmap "니",		$02, $cf
	charmap "닉",		$02, $d0
	charmap "닌",		$02, $d1
	charmap "닐",		$02, $d2
	charmap "닒",		$02, $d3
	charmap "님",		$02, $d4
	charmap "닙",		$02, $d5
	charmap "닛",		$02, $d6
	charmap "닝",		$02, $d7
	charmap "닢",		$02, $d8
	charmap "다",		$02, $d9
	charmap "닥",		$02, $da
	charmap "닦",		$02, $db
	charmap "단",		$02, $dc
	charmap "닫",		$02, $dd
	charmap "달",		$02, $de
	charmap "닭",		$02, $df
	charmap "닮",		$02, $e0
	charmap "닯",		$02, $e1
	charmap "닳",		$02, $e2
	charmap "담",		$02, $e3
	charmap "답",		$02, $e4
	charmap "닷",		$02, $e5
	charmap "닸",		$02, $e6
	charmap "당",		$02, $e7
	charmap "닺",		$02, $e8
	charmap "닻",		$02, $e9
	charmap "닿",		$02, $ea
	charmap "대",		$02, $eb
	charmap "댁",		$02, $ec
	charmap "댄",		$02, $ed
	charmap "댈",		$02, $ee
	charmap "댐",		$02, $ef
	charmap "댑",		$02, $f0
	charmap "댓",		$02, $f1
	charmap "댔",		$02, $f2
	charmap "댕",		$02, $f3
	charmap "더",		$02, $f5
	charmap "덕",		$02, $f6
	charmap "덖",		$02, $f7
	charmap "던",		$02, $f8
	charmap "덛",		$02, $f9
	charmap "덜",		$02, $fa
	charmap "덞",		$02, $fb
	charmap "덟",		$02, $fc
	charmap "덤",		$02, $fd
	charmap "덥",		$02, $fe

	charmap "덧",		$03, $01
	charmap "덩",		$03, $02
	charmap "덫",		$03, $03
	charmap "덮",		$03, $04
	charmap "데",		$03, $05
	charmap "덱",		$03, $06
	charmap "덴",		$03, $07
	charmap "델",		$03, $08
	charmap "뎀",		$03, $09
	charmap "뎁",		$03, $0a
	charmap "뎃",		$03, $0b
	charmap "뎄",		$03, $0c
	charmap "뎅",		$03, $0d
	charmap "뎌",		$03, $0e
	charmap "뎐",		$03, $0f
	charmap "뎔",		$03, $10
	charmap "뎠",		$03, $11
	charmap "뎡",		$03, $12
	charmap "뎨",		$03, $13
	charmap "뎬",		$03, $14
	charmap "도",		$03, $15
	charmap "독",		$03, $16
	charmap "돈",		$03, $17
	charmap "돋",		$03, $18
	charmap "돌",		$03, $19
	charmap "돎",		$03, $1a
	charmap "돔",		$03, $1c
	charmap "돕",		$03, $1d
	charmap "돗",		$03, $1e
	charmap "동",		$03, $1f
	charmap "돛",		$03, $20
	charmap "돝",		$03, $21
	charmap "돠",		$03, $22
	charmap "돤",		$03, $23
	charmap "돨",		$03, $24
	charmap "돼",		$03, $25
	charmap "됐",		$03, $26
	charmap "되",		$03, $27
	charmap "된",		$03, $28
	charmap "될",		$03, $29
	charmap "됨",		$03, $2a
	charmap "됩",		$03, $2b
	charmap "됫",		$03, $2c
	charmap "됴",		$03, $2d
	charmap "두",		$03, $2e
	charmap "둑",		$03, $2f
	charmap "둔",		$03, $30
	charmap "둘",		$03, $31
	charmap "둠",		$03, $32
	charmap "둡",		$03, $33
	charmap "둣",		$03, $34
	charmap "둥",		$03, $35
	charmap "둬",		$03, $36
	charmap "뒀",		$03, $37
	charmap "뒈",		$03, $38
	charmap "뒝",		$03, $39
	charmap "뒤",		$03, $3a
	charmap "뒨",		$03, $3b
	charmap "뒬",		$03, $3c
	charmap "뒵",		$03, $3d
	charmap "뒷",		$03, $3e
	charmap "뒹",		$03, $3f
	charmap "듀",		$03, $40
	charmap "듄",		$03, $41
	charmap "듈",		$03, $42
	charmap "듐",		$03, $43
	charmap "듕",		$03, $44
	charmap "드",		$03, $45
	charmap "득",		$03, $46
	charmap "든",		$03, $47
	charmap "듣",		$03, $48
	charmap "들",		$03, $49
	charmap "듦",		$03, $4a
	charmap "듬",		$03, $4b
	charmap "듭",		$03, $4c
	charmap "듯",		$03, $4d
	charmap "등",		$03, $4e
	charmap "듸",		$03, $4f
	charmap "디",		$03, $60
	charmap "딕",		$03, $61
	charmap "딘",		$03, $62
	charmap "딛",		$03, $63
	charmap "딜",		$03, $64
	charmap "딤",		$03, $65
	charmap "딥",		$03, $66
	charmap "딧",		$03, $67
	charmap "딨",		$03, $68
	charmap "딩",		$03, $69
	charmap "딪",		$03, $6a
	charmap "따",		$03, $6b
	charmap "딱",		$03, $6c
	charmap "딴",		$03, $6d
	charmap "딸",		$03, $6e
	charmap "땀",		$03, $71
	charmap "땁",		$03, $72
	charmap "땃",		$03, $73
	charmap "땄",		$03, $74
	charmap "땅",		$03, $75
	charmap "땋",		$03, $76
	charmap "때",		$03, $77
	charmap "땍",		$03, $78
	charmap "땐",		$03, $79
	charmap "땔",		$03, $7a
	charmap "땜",		$03, $7b
	charmap "땝",		$03, $7c
	charmap "땟",		$03, $7d
	charmap "땠",		$03, $7e
	charmap "땡",		$03, $7f
	charmap "떠",		$03, $80
	charmap "떡",		$03, $81
	charmap "떤",		$03, $82
	charmap "떨",		$03, $83
	charmap "떪",		$03, $84
	charmap "떫",		$03, $85
	charmap "떰",		$03, $86
	charmap "떱",		$03, $87
	charmap "떳",		$03, $88
	charmap "떴",		$03, $89
	charmap "떵",		$03, $8a
	charmap "떻",		$03, $8b
	charmap "떼",		$03, $8c
	charmap "떽",		$03, $8d
	charmap "뗀",		$03, $8e
	charmap "뗄",		$03, $8f
	charmap "뗌",		$03, $90
	charmap "뗍",		$03, $91
	charmap "뗏",		$03, $92
	charmap "뗐",		$03, $93
	charmap "뗑",		$03, $94
	charmap "뗘",		$03, $95
	charmap "뗬",		$03, $96
	charmap "또",		$03, $97
	charmap "똑",		$03, $98
	charmap "똔",		$03, $99
	charmap "똘",		$03, $9a
	charmap "똥",		$03, $9b
	charmap "똬",		$03, $9c
	charmap "똴",		$03, $9d
	charmap "뙈",		$03, $9e
	charmap "뙤",		$03, $9f
	charmap "뙨",		$03, $a0
	charmap "뚜",		$03, $a1
	charmap "뚝",		$03, $a2
	charmap "뚠",		$03, $a3
	charmap "뚤",		$03, $a4
	charmap "뚫",		$03, $a5
	charmap "뚬",		$03, $a6
	charmap "뚱",		$03, $a7
	charmap "뛔",		$03, $a8
	charmap "뛰",		$03, $a9
	charmap "뛴",		$03, $aa
	charmap "뛸",		$03, $ab
	charmap "뜀",		$03, $ac
	charmap "뜁",		$03, $ad
	charmap "뜅",		$03, $ae
	charmap "뜨",		$03, $af
	charmap "뜩",		$03, $b0
	charmap "뜬",		$03, $b1
	charmap "뜯",		$03, $b2
	charmap "뜰",		$03, $b3
	charmap "뜸",		$03, $b4
	charmap "뜹",		$03, $b5
	charmap "뜻",		$03, $b6
	charmap "띄",		$03, $b7
	charmap "띈",		$03, $b8
	charmap "띌",		$03, $b9
	charmap "띔",		$03, $ba
	charmap "띕",		$03, $bb
	charmap "띠",		$03, $bc
	charmap "띤",		$03, $bd
	charmap "띨",		$03, $be
	charmap "띰",		$03, $bf
	charmap "띱",		$03, $c0
	charmap "띳",		$03, $c1
	charmap "띵",		$03, $c2
	charmap "라",		$03, $c3
	charmap "락",		$03, $c4
	charmap "란",		$03, $c5
	charmap "랄",		$03, $c6
	charmap "람",		$03, $c7
	charmap "랍",		$03, $c8
	charmap "랏",		$03, $c9
	charmap "랐",		$03, $ca
	charmap "랑",		$03, $cb
	charmap "랒",		$03, $cc
	charmap "랖",		$03, $cd
	charmap "랗",		$03, $ce
	charmap "뢔",		$03, $d0
	charmap "래",		$03, $d1
	charmap "랙",		$03, $d2
	charmap "랜",		$03, $d3
	charmap "랠",		$03, $d4
	charmap "램",		$03, $d5
	charmap "랩",		$03, $d6
	charmap "랫",		$03, $d7
	charmap "랬",		$03, $d8
	charmap "랭",		$03, $d9
	charmap "랴",		$03, $da
	charmap "략",		$03, $db
	charmap "랸",		$03, $dc
	charmap "럇",		$03, $dd
	charmap "량",		$03, $de
	charmap "러",		$03, $df
	charmap "럭",		$03, $e0
	charmap "런",		$03, $e1
	charmap "럴",		$03, $e2
	charmap "럼",		$03, $e3
	charmap "럽",		$03, $e4
	charmap "럿",		$03, $e5
	charmap "렀",		$03, $e6
	charmap "렁",		$03, $e7
	charmap "렇",		$03, $e8
	charmap "레",		$03, $e9
	charmap "렉",		$03, $ea
	charmap "렌",		$03, $eb
	charmap "렐",		$03, $ec
	charmap "렘",		$03, $ed
	charmap "렙",		$03, $ee
	charmap "렛",		$03, $ef
	charmap "렝",		$03, $f0
	charmap "려",		$03, $f1
	charmap "력",		$03, $f2
	charmap "련",		$03, $f3
	charmap "렬",		$03, $f4
	charmap "렴",		$03, $f5
	charmap "렵",		$03, $f6
	charmap "렷",		$03, $f7
	charmap "렸",		$03, $f8
	charmap "령",		$03, $f9
	charmap "례",		$03, $fa
	charmap "롄",		$03, $fb
	charmap "롑",		$03, $fc
	charmap "롓",		$03, $fd
	charmap "로",		$03, $fe
	charmap "록",		$03, $ff

	charmap "론",		$04, $00
	charmap "롤",		$04, $01
	charmap "롬",		$04, $02
	charmap "롭",		$04, $03
	charmap "롯",		$04, $04
	charmap "롱",		$04, $05
	charmap "롸",		$04, $06
	charmap "롼",		$04, $07
	charmap "뢍",		$04, $08
	charmap "뢨",		$04, $09
	charmap "뢰",		$04, $0a
	charmap "뢴",		$04, $0b
	charmap "뢸",		$04, $0c
	charmap "룀",		$04, $0d
	charmap "룁",		$04, $0e
	charmap "룃",		$04, $0f
	charmap "룅",		$04, $10
	charmap "료",		$04, $11
	charmap "룐",		$04, $12
	charmap "룔",		$04, $13
	charmap "룝",		$04, $14
	charmap "룟",		$04, $15
	charmap "룡",		$04, $16
	charmap "루",		$04, $17
	charmap "룩",		$04, $18
	charmap "룬",		$04, $19
	charmap "룰",		$04, $1a
	charmap "룸",		$04, $1b
	charmap "룹",		$04, $1c
	charmap "룻",		$04, $1d
	charmap "룽",		$04, $1e
	charmap "뤄",		$04, $1f
	charmap "뤘",		$04, $20
	charmap "뤠",		$04, $21
	charmap "뤼",		$04, $22
	charmap "뤽",		$04, $23
	charmap "륀",		$04, $24
	charmap "륄",		$04, $25
	charmap "륌",		$04, $26
	charmap "륏",		$04, $27
	charmap "륑",		$04, $28
	charmap "류",		$04, $29
	charmap "륙",		$04, $2a
	charmap "륜",		$04, $2b
	charmap "률",		$04, $2c
	charmap "륨",		$04, $2d
	charmap "륩",		$04, $2e
	charmap "륫",		$04, $31
	charmap "륭",		$04, $32
	charmap "르",		$04, $33
	charmap "륵",		$04, $34
	charmap "른",		$04, $35
	charmap "를",		$04, $36
	charmap "름",		$04, $37
	charmap "릅",		$04, $38
	charmap "릇",		$04, $39
	charmap "릉",		$04, $3a
	charmap "릊",		$04, $3b
	charmap "릍",		$04, $3c
	charmap "릎",		$04, $3d
	charmap "리",		$04, $3e
	charmap "릭",		$04, $3f
	charmap "린",		$04, $40
	charmap "릴",		$04, $41
	charmap "림",		$04, $42
	charmap "립",		$04, $43
	charmap "릿",		$04, $44
	charmap "링",		$04, $45
	charmap "마",		$04, $46
	charmap "막",		$04, $47
	charmap "만",		$04, $48
	charmap "많",		$04, $49
	charmap "맏",		$04, $4a
	charmap "말",		$04, $4b
	charmap "맑",		$04, $4c
	charmap "맒",		$04, $4d
	charmap "맘",		$04, $4e
	charmap "맙",		$04, $4f
	charmap "맛",		$04, $60
	charmap "망",		$04, $61
	charmap "맞",		$04, $62
	charmap "맡",		$04, $63
	charmap "맣",		$04, $64
	charmap "매",		$04, $65
	charmap "맥",		$04, $66
	charmap "맨",		$04, $67
	charmap "맬",		$04, $68
	charmap "맴",		$04, $69
	charmap "맵",		$04, $6a
	charmap "맷",		$04, $6b
	charmap "맸",		$04, $6c
	charmap "맹",		$04, $6d
	charmap "맺",		$04, $6e
	charmap "먀",		$04, $6f
	charmap "먁",		$04, $70
	charmap "먈",		$04, $71
	charmap "먕",		$04, $72
	charmap "머",		$04, $73
	charmap "먹",		$04, $74
	charmap "먼",		$04, $75
	charmap "멀",		$04, $76
	charmap "멂",		$04, $77
	charmap "멈",		$04, $78
	charmap "멉",		$04, $79
	charmap "멋",		$04, $7a
	charmap "멍",		$04, $7b
	charmap "멎",		$04, $7c
	charmap "멓",		$04, $7d
	charmap "메",		$04, $7e
	charmap "멕",		$04, $7f
	charmap "멘",		$04, $80
	charmap "멜",		$04, $81
	charmap "멤",		$04, $82
	charmap "멥",		$04, $83
	charmap "멧",		$04, $84
	charmap "멨",		$04, $85
	charmap "멩",		$04, $86
	charmap "며",		$04, $87
	charmap "멱",		$04, $88
	charmap "면",		$04, $89
	charmap "멸",		$04, $8a
	charmap "몃",		$04, $8b
	charmap "몄",		$04, $8c
	charmap "명",		$04, $8d
	charmap "몇",		$04, $8e
	charmap "몌",		$04, $8f
	charmap "모",		$04, $90
	charmap "목",		$04, $91
	charmap "몫",		$04, $92
	charmap "몬",		$04, $93
	charmap "몰",		$04, $94
	charmap "몲",		$04, $95
	charmap "몸",		$04, $96
	charmap "몹",		$04, $97
	charmap "못",		$04, $98
	charmap "몽",		$04, $99
	charmap "뫄",		$04, $9a
	charmap "뫈",		$04, $9b
	charmap "뫘",		$04, $9c
	charmap "뫙",		$04, $9d
	charmap "뫼",		$04, $9e
	charmap "묀",		$04, $a1
	charmap "묄",		$04, $a2
	charmap "묍",		$04, $a3
	charmap "묏",		$04, $a4
	charmap "묑",		$04, $a5
	charmap "묘",		$04, $a6
	charmap "묜",		$04, $a7
	charmap "묠",		$04, $a8
	charmap "묩",		$04, $a9
	charmap "묫",		$04, $aa
	charmap "무",		$04, $ab
	charmap "묵",		$04, $ac
	charmap "묶",		$04, $ad
	charmap "문",		$04, $ae
	charmap "묻",		$04, $af
	charmap "물",		$04, $b0
	charmap "묽",		$04, $b1
	charmap "묾",		$04, $b2
	charmap "뭄",		$04, $b3
	charmap "뭅",		$04, $b4
	charmap "뭇",		$04, $b5
	charmap "뭉",		$04, $b6
	charmap "뭍",		$04, $b7
	charmap "뭏",		$04, $b8
	charmap "뭐",		$04, $b9
	charmap "뭔",		$04, $ba
	charmap "뭘",		$04, $bb
	charmap "뭡",		$04, $bc
	charmap "뭣",		$04, $bd
	charmap "뭬",		$04, $be
	charmap "뮈",		$04, $bf
	charmap "뮌",		$04, $c0
	charmap "뮐",		$04, $c1
	charmap "뮤",		$04, $c2
	charmap "뮨",		$04, $c3
	charmap "뮬",		$04, $c4
	charmap "뮴",		$04, $c5
	charmap "뮷",		$04, $c6
	charmap "므",		$04, $c7
	charmap "믄",		$04, $c8
	charmap "믈",		$04, $c9
	charmap "믐",		$04, $ca
	charmap "믓",		$04, $cb
	charmap "미",		$04, $cc
	charmap "믹",		$04, $cd
	charmap "민",		$04, $ce
	charmap "믿",		$04, $cf
	charmap "밀",		$04, $d0
	charmap "밂",		$04, $d1
	charmap "밈",		$04, $d2
	charmap "밉",		$04, $d3
	charmap "밋",		$04, $d4
	charmap "밌",		$04, $d5
	charmap "밍",		$04, $d6
	charmap "및",		$04, $d7
	charmap "밑",		$04, $d8
	charmap "바",		$04, $d9
	charmap "박",		$04, $da
	charmap "밖",		$04, $db
	charmap "밗",		$04, $dc
	charmap "반",		$04, $dd
	charmap "받",		$04, $de
	charmap "발",		$04, $df
	charmap "밝",		$04, $e0
	charmap "밞",		$04, $e1
	charmap "밟",		$04, $e2
	charmap "밤",		$04, $e3
	charmap "밥",		$04, $e4
	charmap "밧",		$04, $e5
	charmap "방",		$04, $e6
	charmap "밭",		$04, $e7
	charmap "배",		$04, $e8
	charmap "백",		$04, $e9
	charmap "밴",		$04, $ea
	charmap "밸",		$04, $eb
	charmap "뱀",		$04, $ec
	charmap "뱁",		$04, $ed
	charmap "뱃",		$04, $ee
	charmap "뱄",		$04, $ef
	charmap "뱅",		$04, $f0
	charmap "뱉",		$04, $f1
	charmap "뱌",		$04, $f2
	charmap "뱍",		$04, $f3
	charmap "뱐",		$04, $f4
	charmap "뱝",		$04, $f5
	charmap "버",		$04, $f6
	charmap "벅",		$04, $f7
	charmap "번",		$04, $f8
	charmap "벋",		$04, $f9
	charmap "벌",		$04, $fa
	charmap "벎",		$04, $fb
	charmap "범",		$04, $fc
	charmap "법",		$04, $fd
	charmap "벗",		$04, $fe

	charmap "벙",		$05, $01
	charmap "벚",		$05, $02
	charmap "베",		$05, $03
	charmap "벡",		$05, $04
	charmap "벤",		$05, $05
	charmap "벧",		$05, $06
	charmap "벨",		$05, $07
	charmap "벰",		$05, $08
	charmap "벱",		$05, $09
	charmap "벳",		$05, $0a
	charmap "벵",		$05, $0c
	charmap "벴",		$05, $0b
	charmap "벼",		$05, $0d
	charmap "벽",		$05, $0e
	charmap "변",		$05, $0f
	charmap "별",		$05, $10
	charmap "볍",		$05, $11
	charmap "볏",		$05, $12
	charmap "병",		$05, $14
	charmap "볕",		$05, $15
	charmap "볐",		$05, $13
	charmap "볘",		$05, $16
	charmap "볜",		$05, $17
	charmap "보",		$05, $18
	charmap "복",		$05, $19
	charmap "본",		$05, $1b
	charmap "볼",		$05, $1c
	charmap "봄",		$05, $1d
	charmap "봅",		$05, $1e
	charmap "봇",		$05, $1f
	charmap "봉",		$05, $20
	charmap "볶",		$05, $1a
	charmap "봐",		$05, $21
	charmap "봔",		$05, $22
	charmap "봤",		$05, $23
	charmap "봬",		$05, $24
	charmap "뵀",		$05, $25
	charmap "뵈",		$05, $26
	charmap "뵉",		$05, $27
	charmap "뵌",		$05, $28
	charmap "뵐",		$05, $29
	charmap "뵘",		$05, $2a
	charmap "뵙",		$05, $2b
	charmap "뵤",		$05, $2c
	charmap "뵨",		$05, $2d
	charmap "부",		$05, $2e
	charmap "북",		$05, $2f
	charmap "분",		$05, $30
	charmap "붇",		$05, $31
	charmap "불",		$05, $32
	charmap "붐",		$05, $35
	charmap "붑",		$05, $36
	charmap "붓",		$05, $37
	charmap "붕",		$05, $38
	charmap "붙",		$05, $39
	charmap "붚",		$05, $3a
	charmap "붉",		$05, $33
	charmap "붊",		$05, $34
	charmap "붜",		$05, $3b
	charmap "붤",		$05, $3c
	charmap "붰",		$05, $3d
	charmap "붸",		$05, $3e
	charmap "뷔",		$05, $3f
	charmap "뷕",		$05, $40
	charmap "뷘",		$05, $41
	charmap "뷜",		$05, $42
	charmap "뷩",		$05, $43
	charmap "뷰",		$05, $44
	charmap "뷴",		$05, $45
	charmap "뷸",		$05, $46
	charmap "븀",		$05, $47
	charmap "븃",		$05, $48
	charmap "븅",		$05, $49
	charmap "브",		$05, $4a
	charmap "븍",		$05, $4b
	charmap "븐",		$05, $4c
	charmap "블",		$05, $4d
	charmap "븜",		$05, $4e
	charmap "븝",		$05, $4f
	charmap "븟",		$05, $60
	charmap "비",		$05, $61
	charmap "빅",		$05, $62
	charmap "빈",		$05, $63
	charmap "빌",		$05, $64
	charmap "빎",		$05, $65
	charmap "빔",		$05, $66
	charmap "빕",		$05, $67
	charmap "빗",		$05, $68
	charmap "빙",		$05, $69
	charmap "빚",		$05, $6a
	charmap "빛",		$05, $6b
	charmap "빠",		$05, $6c
	charmap "빡",		$05, $6d
	charmap "빤",		$05, $6e
	charmap "빨",		$05, $71
	charmap "빪",		$05, $72
	charmap "빰",		$05, $73
	charmap "빱",		$05, $74
	charmap "빳",		$05, $75
	charmap "빴",		$05, $76
	charmap "빵",		$05, $77
	charmap "빻",		$05, $78
	charmap "빼",		$05, $79
	charmap "빽",		$05, $7a
	charmap "뺀",		$05, $7b
	charmap "뺄",		$05, $7c
	charmap "뺌",		$05, $7d
	charmap "뺍",		$05, $7e
	charmap "뺏",		$05, $7f
	charmap "뺐",		$05, $80
	charmap "뺑",		$05, $81
	charmap "뺘",		$05, $82
	charmap "뺙",		$05, $83
	charmap "뺨",		$05, $84
	charmap "뻐",		$05, $85
	charmap "뻑",		$05, $86
	charmap "뻔",		$05, $87
	charmap "뻗",		$05, $88
	charmap "뻘",		$05, $89
	charmap "뻠",		$05, $8a
	charmap "뻣",		$05, $8b
	charmap "뻤",		$05, $8c
	charmap "뻥",		$05, $8d
	charmap "뻬",		$05, $8e
	charmap "뼁",		$05, $8f
	charmap "뼈",		$05, $90
	charmap "뼉",		$05, $91
	charmap "뼘",		$05, $92
	charmap "뼙",		$05, $93
	charmap "뼛",		$05, $94
	charmap "뼜",		$05, $95
	charmap "뼝",		$05, $96
	charmap "뽀",		$05, $97
	charmap "뽁",		$05, $98
	charmap "뽄",		$05, $99
	charmap "뽈",		$05, $9a
	charmap "뽐",		$05, $9b
	charmap "뽑",		$05, $9c
	charmap "뽕",		$05, $9d
	charmap "뾔",		$05, $9e
	charmap "뾰",		$05, $9f
	charmap "뿅",		$05, $a0
	charmap "뿌",		$05, $a1
	charmap "뿍",		$05, $a2
	charmap "뿐",		$05, $a3
	charmap "뿔",		$05, $a4
	charmap "뿜",		$05, $a5
	charmap "뿟",		$05, $a6
	charmap "뿡",		$05, $a7
	charmap "쀼",		$05, $a8
	charmap "쁑",		$05, $a9
	charmap "쁘",		$05, $aa
	charmap "쁜",		$05, $ab
	charmap "쁠",		$05, $ac
	charmap "쁨",		$05, $ad
	charmap "쁩",		$05, $ae
	charmap "삐",		$05, $af
	charmap "삑",		$05, $b0
	charmap "삔",		$05, $b1
	charmap "삘",		$05, $b2
	charmap "삠",		$05, $b3
	charmap "삡",		$05, $b4
	charmap "삣",		$05, $b5
	charmap "삥",		$05, $b6
	charmap "사",		$05, $b7
	charmap "삭",		$05, $b8
	charmap "삯",		$05, $b9
	charmap "산",		$05, $ba
	charmap "삳",		$05, $bb
	charmap "살",		$05, $bc
	charmap "삵",		$05, $bd
	charmap "삶",		$05, $be
	charmap "삼",		$05, $bf
	charmap "삽",		$05, $c0
	charmap "삿",		$05, $c1
	charmap "샀",		$05, $c2
	charmap "상",		$05, $c3
	charmap "샅",		$05, $c4
	charmap "새",		$05, $c5
	charmap "색",		$05, $c6
	charmap "샌",		$05, $c7
	charmap "샐",		$05, $c8
	charmap "샘",		$05, $c9
	charmap "샙",		$05, $ca
	charmap "샛",		$05, $cb
	charmap "샜",		$05, $cc
	charmap "생",		$05, $cd
	charmap "샤",		$05, $ce
	charmap "샥",		$05, $d1
	charmap "샨",		$05, $d2
	charmap "샬",		$05, $d3
	charmap "샴",		$05, $d4
	charmap "샵",		$05, $d5
	charmap "샷",		$05, $d6
	charmap "샹",		$05, $d7
	charmap "섀",		$05, $d8
	charmap "섄",		$05, $d9
	charmap "섈",		$05, $da
	charmap "섐",		$05, $db
	charmap "섕",		$05, $dc
	charmap "서",		$05, $dd
	charmap "석",		$05, $de
	charmap "섞",		$05, $df
	charmap "섟",		$05, $e0
	charmap "선",		$05, $e1
	charmap "섣",		$05, $e2
	charmap "설",		$05, $e3
	charmap "섦",		$05, $e4
	charmap "섧",		$05, $e5
	charmap "섬",		$05, $e6
	charmap "섭",		$05, $e7
	charmap "섯",		$05, $e8
	charmap "섰",		$05, $e9
	charmap "성",		$05, $ea
	charmap "섶",		$05, $eb
	charmap "세",		$05, $ec
	charmap "섹",		$05, $ed
	charmap "센",		$05, $ee
	charmap "셀",		$05, $ef
	charmap "셈",		$05, $f0
	charmap "셉",		$05, $f1
	charmap "셋",		$05, $f2
	charmap "셌",		$05, $f3
	charmap "셍",		$05, $f4
	charmap "셔",		$05, $f5
	charmap "셕",		$05, $f6
	charmap "션",		$05, $f7
	charmap "셜",		$05, $f8
	charmap "셤",		$05, $f9
	charmap "셥",		$05, $fa
	charmap "셧",		$05, $fb
	charmap "셨",		$05, $fc
	charmap "셩",		$05, $fd
	charmap "셰",		$05, $fe
	charmap "셴",		$05, $ff

	charmap "셸",		$06, $00
	charmap "솅",		$06, $01
	charmap "소",		$06, $02
	charmap "속",		$06, $03
	charmap "솎",		$06, $04
	charmap "손",		$06, $05
	charmap "솔",		$06, $06
	charmap "솖",		$06, $07
	charmap "솜",		$06, $08
	charmap "솝",		$06, $09
	charmap "솟",		$06, $0a
	charmap "송",		$06, $0b
	charmap "솥",		$06, $0c
	charmap "솨",		$06, $0d
	charmap "솩",		$06, $0e
	charmap "솬",		$06, $0f
	charmap "솰",		$06, $10
	charmap "솽",		$06, $11
	charmap "쇄",		$06, $12
	charmap "쇈",		$06, $13
	charmap "쇌",		$06, $14
	charmap "쇔",		$06, $15
	charmap "쇗",		$06, $16
	charmap "쇘",		$06, $17
	charmap "쇠",		$06, $18
	charmap "쇤",		$06, $19
	charmap "쇨",		$06, $1a
	charmap "쇰",		$06, $1b
	charmap "쇱",		$06, $1c
	charmap "쇳",		$06, $1d
	charmap "쇼",		$06, $1e
	charmap "쇽",		$06, $1f
	charmap "숀",		$06, $20
	charmap "숄",		$06, $21
	charmap "숌",		$06, $22
	charmap "숍",		$06, $23
	charmap "숏",		$06, $24
	charmap "숑",		$06, $25
	charmap "수",		$06, $26
	charmap "숙",		$06, $27
	charmap "순",		$06, $28
	charmap "숟",		$06, $29
	charmap "술",		$06, $2a
	charmap "숨",		$06, $2b
	charmap "숩",		$06, $2c
	charmap "숫",		$06, $2d
	charmap "숭",		$06, $2e
	charmap "쌰",		$06, $2f
	charmap "쎼",		$06, $30
	charmap "숯",		$06, $31
	charmap "숱",		$06, $32
	charmap "숲",		$06, $33
	charmap "숴",		$06, $34
	charmap "쉈",		$06, $35
	charmap "쉐",		$06, $36
	charmap "쉑",		$06, $37
	charmap "쉔",		$06, $38
	charmap "쉘",		$06, $39
	charmap "쉠",		$06, $3a
	charmap "쉥",		$06, $3b
	charmap "쉬",		$06, $3c
	charmap "쉭",		$06, $3d
	charmap "쉰",		$06, $3e
	charmap "쉴",		$06, $3f
	charmap "쉼",		$06, $40
	charmap "쉽",		$06, $41
	charmap "쉿",		$06, $42
	charmap "슁",		$06, $43
	charmap "슈",		$06, $44
	charmap "슉",		$06, $45
	charmap "슐",		$06, $46
	charmap "슘",		$06, $47
	charmap "슛",		$06, $48
	charmap "슝",		$06, $49
	charmap "스",		$06, $4a
	charmap "슥",		$06, $4b
	charmap "슨",		$06, $4c
	charmap "슬",		$06, $4d
	charmap "슭",		$06, $4e
	charmap "슴",		$06, $4f
	charmap "습",		$06, $60
	charmap "슷",		$06, $61
	charmap "승",		$06, $62
	charmap "시",		$06, $63
	charmap "식",		$06, $64
	charmap "신",		$06, $65
	charmap "싣",		$06, $66
	charmap "실",		$06, $67
	charmap "싫",		$06, $68
	charmap "심",		$06, $69
	charmap "십",		$06, $6a
	charmap "싯",		$06, $6b
	charmap "싱",		$06, $6c
	charmap "싶",		$06, $6d
	charmap "싸",		$06, $6e
	charmap "싹",		$06, $6f
	charmap "싻",		$06, $70
	charmap "싼",		$06, $71
	charmap "쌀",		$06, $72
	charmap "쌈",		$06, $73
	charmap "쌉",		$06, $74
	charmap "쌌",		$06, $75
	charmap "쌍",		$06, $76
	charmap "쌓",		$06, $77
	charmap "쌔",		$06, $78
	charmap "쌕",		$06, $79
	charmap "쌘",		$06, $7a
	charmap "쌜",		$06, $7b
	charmap "쌤",		$06, $7c
	charmap "쌥",		$06, $7d
	charmap "쌨",		$06, $7e
	charmap "쌩",		$06, $7f
	charmap "썅",		$06, $80
	charmap "써",		$06, $81
	charmap "썩",		$06, $82
	charmap "썬",		$06, $83
	charmap "썰",		$06, $84
	charmap "썲",		$06, $85
	charmap "썸",		$06, $86
	charmap "썹",		$06, $87
	charmap "썼",		$06, $88
	charmap "썽",		$06, $89
	charmap "쎄",		$06, $8a
	charmap "쎈",		$06, $8b
	charmap "쎌",		$06, $8c
	charmap "쏀",		$06, $8d
	charmap "쏘",		$06, $8e
	charmap "쏙",		$06, $8f
	charmap "쏜",		$06, $90
	charmap "쏟",		$06, $91
	charmap "쏠",		$06, $92
	charmap "쏢",		$06, $93
	charmap "쏨",		$06, $94
	charmap "쏩",		$06, $95
	charmap "쏭",		$06, $96
	charmap "쏴",		$06, $97
	charmap "쏵",		$06, $98
	charmap "쏸",		$06, $99
	charmap "쐈",		$06, $9a
	charmap "쐐",		$06, $9b
	charmap "쐤",		$06, $9c
	charmap "쐬",		$06, $9d
	charmap "쐰",		$06, $9e
	charmap "쓔",		$06, $a0
	charmap "쐴",		$06, $a1
	charmap "쐼",		$06, $a2
	charmap "쐽",		$06, $a3
	charmap "쑈",		$06, $a4
	charmap "쑤",		$06, $a5
	charmap "쑥",		$06, $a6
	charmap "쑨",		$06, $a7
	charmap "쑬",		$06, $a8
	charmap "쑴",		$06, $a9
	charmap "쑵",		$06, $aa
	charmap "쑹",		$06, $ab
	charmap "쒀",		$06, $ac
	charmap "쒔",		$06, $ad
	charmap "쒜",		$06, $ae
	charmap "쒸",		$06, $af
	charmap "쒼",		$06, $b0
	charmap "쓩",		$06, $b1
	charmap "쓰",		$06, $b2
	charmap "쓱",		$06, $b3
	charmap "쓴",		$06, $b4
	charmap "쓸",		$06, $b5
	charmap "쓺",		$06, $b6
	charmap "쓿",		$06, $b7
	charmap "씀",		$06, $b8
	charmap "씁",		$06, $b9
	charmap "씌",		$06, $ba
	charmap "씐",		$06, $bb
	charmap "씔",		$06, $bc
	charmap "씜",		$06, $bd
	charmap "씨",		$06, $be
	charmap "씩",		$06, $bf
	charmap "씬",		$06, $c0
	charmap "씰",		$06, $c1
	charmap "씸",		$06, $c2
	charmap "씹",		$06, $c3
	charmap "씻",		$06, $c4
	charmap "씽",		$06, $c5
	charmap "아",		$06, $c6
	charmap "악",		$06, $c7
	charmap "안",		$06, $c8
	charmap "앉",		$06, $c9
	charmap "않",		$06, $ca
	charmap "알",		$06, $cb
	charmap "앍",		$06, $cc
	charmap "앎",		$06, $cd
	charmap "앓",		$06, $ce
	charmap "암",		$06, $cf
	charmap "압",		$06, $d0
	charmap "앗",		$06, $d1
	charmap "았",		$06, $d2
	charmap "앙",		$06, $d3
	charmap "앝",		$06, $d4
	charmap "앞",		$06, $d5
	charmap "애",		$06, $d6
	charmap "액",		$06, $d7
	charmap "앤",		$06, $d8
	charmap "앨",		$06, $d9
	charmap "앰",		$06, $da
	charmap "앱",		$06, $db
	charmap "앳",		$06, $dc
	charmap "앴",		$06, $dd
	charmap "앵",		$06, $de
	charmap "야",		$06, $df
	charmap "약",		$06, $e0
	charmap "얀",		$06, $e1
	charmap "얄",		$06, $e2
	charmap "얇",		$06, $e3
	charmap "얌",		$06, $e4
	charmap "얍",		$06, $e5
	charmap "얏",		$06, $e6
	charmap "양",		$06, $e7
	charmap "얕",		$06, $e8
	charmap "얗",		$06, $e9
	charmap "얘",		$06, $ea
	charmap "얜",		$06, $eb
	charmap "얠",		$06, $ec
	charmap "얩",		$06, $ed
	charmap "어",		$06, $ee
	charmap "억",		$06, $ef
	charmap "언",		$06, $f0
	charmap "얹",		$06, $f1
	charmap "얻",		$06, $f2
	charmap "얼",		$06, $f3
	charmap "얽",		$06, $f4
	charmap "얾",		$06, $f5
	charmap "엄",		$06, $f6
	charmap "업",		$06, $f7
	charmap "없",		$06, $f8
	charmap "엇",		$06, $f9
	charmap "었",		$06, $fa
	charmap "엉",		$06, $fb
	charmap "엊",		$06, $fc
	charmap "엌",		$06, $fd
	charmap "엎",		$06, $fe

	charmap "에",		$07, $01
	charmap "엑",		$07, $02
	charmap "엔",		$07, $03
	charmap "엘",		$07, $04
	charmap "엠",		$07, $05
	charmap "엡",		$07, $06
	charmap "엣",		$07, $07
	charmap "엥",		$07, $08
	charmap "여",		$07, $09
	charmap "역",		$07, $0a
	charmap "엮",		$07, $0b
	charmap "연",		$07, $0c
	charmap "열",		$07, $0d
	charmap "엶",		$07, $0e
	charmap "엷",		$07, $0f
	charmap "염",		$07, $10
	charmap "엽",		$07, $11
	charmap "엾",		$07, $12
	charmap "엿",		$07, $13
	charmap "였",		$07, $14
	charmap "영",		$07, $15
	charmap "옅",		$07, $16
	charmap "옆",		$07, $17
	charmap "옇",		$07, $18
	charmap "예",		$07, $19
	charmap "옌",		$07, $1a
	charmap "옐",		$07, $1b
	charmap "옘",		$07, $1c
	charmap "옙",		$07, $1d
	charmap "옛",		$07, $1e
	charmap "옜",		$07, $1f
	charmap "오",		$07, $20
	charmap "옥",		$07, $21
	charmap "온",		$07, $22
	charmap "올",		$07, $23
	charmap "옭",		$07, $24
	charmap "옮",		$07, $25
	charmap "옰",		$07, $26
	charmap "옳",		$07, $27
	charmap "옴",		$07, $28
	charmap "옵",		$07, $29
	charmap "옷",		$07, $2a
	charmap "옹",		$07, $2b
	charmap "옻",		$07, $2c
	charmap "와",		$07, $2d
	charmap "왁",		$07, $2e
	charmap "완",		$07, $2f
	charmap "왈",		$07, $30
	charmap "왐",		$07, $31
	charmap "왑",		$07, $32
	charmap "왓",		$07, $33
	charmap "왔",		$07, $34
	charmap "왕",		$07, $35
	charmap "왜",		$07, $36
	charmap "왝",		$07, $37
	charmap "왠",		$07, $38
	charmap "왬",		$07, $39
	charmap "왯",		$07, $3a
	charmap "왱",		$07, $3b
	charmap "외",		$07, $3c
	charmap "왹",		$07, $3d
	charmap "왼",		$07, $3e
	charmap "욀",		$07, $3f
	charmap "욈",		$07, $40
	charmap "욉",		$07, $41
	charmap "욋",		$07, $42
	charmap "욍",		$07, $43
	charmap "요",		$07, $44
	charmap "욕",		$07, $45
	charmap "욘",		$07, $46
	charmap "욜",		$07, $47
	charmap "욤",		$07, $48
	charmap "욥",		$07, $49
	charmap "욧",		$07, $4a
	charmap "용",		$07, $4b
	charmap "우",		$07, $4c
	charmap "욱",		$07, $4d
	charmap "운",		$07, $4e
	charmap "울",		$07, $4f
	charmap "욹",		$07, $60
	charmap "욺",		$07, $61
	charmap "움",		$07, $62
	charmap "웁",		$07, $63
	charmap "웃",		$07, $64
	charmap "웅",		$07, $65
	charmap "워",		$07, $66
	charmap "웍",		$07, $67
	charmap "원",		$07, $68
	charmap "월",		$07, $69
	charmap "웜",		$07, $6a
	charmap "웝",		$07, $6b
	charmap "웠",		$07, $6c
	charmap "웡",		$07, $6d
	charmap "웨",		$07, $6e
	charmap "웩",		$07, $71
	charmap "웬",		$07, $72
	charmap "웰",		$07, $73
	charmap "웸",		$07, $74
	charmap "웹",		$07, $75
	charmap "웽",		$07, $76
	charmap "위",		$07, $77
	charmap "윅",		$07, $78
	charmap "윈",		$07, $79
	charmap "윌",		$07, $7a
	charmap "윔",		$07, $7b
	charmap "윕",		$07, $7c
	charmap "윗",		$07, $7d
	charmap "윙",		$07, $7e
	charmap "유",		$07, $7f
	charmap "육",		$07, $80
	charmap "윤",		$07, $81
	charmap "율",		$07, $82
	charmap "윰",		$07, $83
	charmap "윱",		$07, $84
	charmap "윳",		$07, $85
	charmap "융",		$07, $86
	charmap "윷",		$07, $87
	charmap "으",		$07, $88
	charmap "윽",		$07, $89
	charmap "은",		$07, $8a
	charmap "을",		$07, $8b
	charmap "읆",		$07, $8c
	charmap "음",		$07, $8d
	charmap "읍",		$07, $8e
	charmap "읏",		$07, $8f
	charmap "응",		$07, $90
	charmap "읒",		$07, $91
	charmap "읓",		$07, $92
	charmap "읔",		$07, $93
	charmap "읕",		$07, $94
	charmap "읖",		$07, $95
	charmap "읗",		$07, $96
	charmap "의",		$07, $97
	charmap "읜",		$07, $98
	charmap "읠",		$07, $99
	charmap "읨",		$07, $9a
	charmap "읫",		$07, $9b
	charmap "이",		$07, $9c
	charmap "익",		$07, $9d
	charmap "인",		$07, $9e
	charmap "일",		$07, $9f
	charmap "읽",		$07, $a0
	charmap "읾",		$07, $a1
	charmap "잃",		$07, $a2
	charmap "임",		$07, $a3
	charmap "입",		$07, $a4
	charmap "잇",		$07, $a5
	charmap "있",		$07, $a6
	charmap "잉",		$07, $a7
	charmap "잊",		$07, $a8
	charmap "잎",		$07, $a9
	charmap "자",		$07, $aa
	charmap "작",		$07, $ab
	charmap "잔",		$07, $ac
	charmap "잖",		$07, $ad
	charmap "잗",		$07, $ae
	charmap "잘",		$07, $af
	charmap "잚",		$07, $b0
	charmap "잠",		$07, $b1
	charmap "잡",		$07, $b2
	charmap "잣",		$07, $b3
	charmap "잤",		$07, $b4
	charmap "장",		$07, $b5
	charmap "잦",		$07, $b6
	charmap "재",		$07, $b7
	charmap "잭",		$07, $b8
	charmap "잰",		$07, $b9
	charmap "잴",		$07, $ba
	charmap "잼",		$07, $bb
	charmap "잽",		$07, $bc
	charmap "잿",		$07, $bd
	charmap "쟀",		$07, $be
	charmap "쟁",		$07, $bf
	charmap "쟈",		$07, $c0
	charmap "쟉",		$07, $c1
	charmap "쟌",		$07, $c2
	charmap "쟎",		$07, $c3
	charmap "쟐",		$07, $c4
	charmap "쟘",		$07, $c5
	charmap "쟝",		$07, $c6
	charmap "쟤",		$07, $c7
	charmap "쟨",		$07, $c8
	charmap "쟬",		$07, $c9
	charmap "저",		$07, $ca
	charmap "적",		$07, $cb
	charmap "전",		$07, $cc
	charmap "절",		$07, $cd
	charmap "젊",		$07, $ce
	charmap "점",		$07, $d1
	charmap "접",		$07, $d2
	charmap "젓",		$07, $d3
	charmap "정",		$07, $d4
	charmap "젖",		$07, $d5
	charmap "제",		$07, $d6
	charmap "젝",		$07, $d7
	charmap "젠",		$07, $d8
	charmap "젤",		$07, $d9
	charmap "젬",		$07, $da
	charmap "젭",		$07, $db
	charmap "젯",		$07, $dc
	charmap "젱",		$07, $dd
	charmap "져",		$07, $de
	charmap "젼",		$07, $df
	charmap "졀",		$07, $e0
	charmap "졈",		$07, $e1
	charmap "졉",		$07, $e2
	charmap "졌",		$07, $e3
	charmap "졍",		$07, $e4
	charmap "졔",		$07, $e5
	charmap "조",		$07, $e6
	charmap "족",		$07, $e7
	charmap "존",		$07, $e8
	charmap "졸",		$07, $e9
	charmap "졺",		$07, $ea
	charmap "좀",		$07, $eb
	charmap "좁",		$07, $ec
	charmap "좃",		$07, $ed
	charmap "종",		$07, $ee
	charmap "좆",		$07, $ef
	charmap "좇",		$07, $f0
	charmap "좋",		$07, $f1
	charmap "좌",		$07, $f2
	charmap "좍",		$07, $f3
	charmap "좔",		$07, $f4
	charmap "좝",		$07, $f5
	charmap "좟",		$07, $f6
	charmap "좡",		$07, $f7
	charmap "좨",		$07, $f8
	charmap "좼",		$07, $f9
	charmap "좽",		$07, $fa
	charmap "죄",		$07, $fb
	charmap "죈",		$07, $fc
	charmap "죌",		$07, $fd
	charmap "죔",		$07, $fe
	charmap "죕",		$07, $ff

	charmap "죗",		$08, $00
	charmap "죙",		$08, $01
	charmap "죠",		$08, $02
	charmap "죡",		$08, $03
	charmap "죤",		$08, $04
	charmap "죵",		$08, $05
	charmap "주",		$08, $06
	charmap "죽",		$08, $07
	charmap "준",		$08, $08
	charmap "줄",		$08, $09
	charmap "줅",		$08, $0a
	charmap "줆",		$08, $0b
	charmap "줌",		$08, $0c
	charmap "줍",		$08, $0d
	charmap "줏",		$08, $0e
	charmap "중",		$08, $0f
	charmap "줘",		$08, $10
	charmap "줬",		$08, $11
	charmap "줴",		$08, $12
	charmap "쥐",		$08, $13
	charmap "쥑",		$08, $14
	charmap "쥔",		$08, $15
	charmap "쥘",		$08, $16
	charmap "쥠",		$08, $17
	charmap "쥡",		$08, $18
	charmap "쥣",		$08, $19
	charmap "쥬",		$08, $1a
	charmap "쥰",		$08, $1b
	charmap "쥴",		$08, $1c
	charmap "쥼",		$08, $1d
	charmap "즈",		$08, $1e
	charmap "즉",		$08, $1f
	charmap "즌",		$08, $20
	charmap "즐",		$08, $21
	charmap "즘",		$08, $22
	charmap "즙",		$08, $23
	charmap "즛",		$08, $24
	charmap "증",		$08, $25
	charmap "지",		$08, $26
	charmap "직",		$08, $27
	charmap "진",		$08, $28
	charmap "짇",		$08, $29
	charmap "질",		$08, $2a
	charmap "짊",		$08, $2b
	charmap "짐",		$08, $2c
	charmap "집",		$08, $2d
	charmap "짓",		$08, $2e
	charmap "쬬",		$08, $30
	charmap "징",		$08, $31
	charmap "짖",		$08, $32
	charmap "짙",		$08, $33
	charmap "짚",		$08, $34
	charmap "짜",		$08, $35
	charmap "짝",		$08, $36
	charmap "짠",		$08, $37
	charmap "짢",		$08, $38
	charmap "짤",		$08, $39
	charmap "짧",		$08, $3a
	charmap "짬",		$08, $3b
	charmap "짭",		$08, $3c
	charmap "짯",		$08, $3d
	charmap "짰",		$08, $3e
	charmap "짱",		$08, $3f
	charmap "째",		$08, $40
	charmap "짹",		$08, $41
	charmap "짼",		$08, $42
	charmap "쨀",		$08, $43
	charmap "쨈",		$08, $44
	charmap "쨉",		$08, $45
	charmap "쨋",		$08, $46
	charmap "쨌",		$08, $47
	charmap "쨍",		$08, $48
	charmap "쨔",		$08, $49
	charmap "쨘",		$08, $4a
	charmap "쨩",		$08, $4b
	charmap "쩌",		$08, $4c
	charmap "쩍",		$08, $4d
	charmap "쩐",		$08, $4e
	charmap "쩔",		$08, $4f
	charmap "쩜",		$08, $60
	charmap "쩝",		$08, $61
	charmap "쩟",		$08, $62
	charmap "쩠",		$08, $63
	charmap "쩡",		$08, $64
	charmap "쩨",		$08, $65
	charmap "쩽",		$08, $66
	charmap "쪄",		$08, $67
	charmap "쪘",		$08, $68
	charmap "쪼",		$08, $69
	charmap "쪽",		$08, $6a
	charmap "쫀",		$08, $6b
	charmap "쫄",		$08, $6c
	charmap "쫌",		$08, $6d
	charmap "쫍",		$08, $6e
	charmap "쫏",		$08, $6f
	charmap "쫑",		$08, $70
	charmap "쫓",		$08, $71
	charmap "쫘",		$08, $72
	charmap "쫙",		$08, $73
	charmap "쫠",		$08, $74
	charmap "쫬",		$08, $75
	charmap "쫴",		$08, $76
	charmap "쬈",		$08, $77
	charmap "쬐",		$08, $78
	charmap "쬔",		$08, $79
	charmap "쬘",		$08, $7a
	charmap "쬠",		$08, $7b
	charmap "쬡",		$08, $7c
	charmap "쭁",		$08, $7d
	charmap "쭈",		$08, $7e
	charmap "쭉",		$08, $7f
	charmap "쭌",		$08, $80
	charmap "쭐",		$08, $81
	charmap "쭘",		$08, $82
	charmap "쭙",		$08, $83
	charmap "쭝",		$08, $84
	charmap "쭤",		$08, $85
	charmap "쭸",		$08, $86
	charmap "쭹",		$08, $87
	charmap "쮜",		$08, $88
	charmap "쮸",		$08, $89
	charmap "쯔",		$08, $8a
	charmap "쯤",		$08, $8b
	charmap "쯧",		$08, $8c
	charmap "쯩",		$08, $8d
	charmap "찌",		$08, $8e
	charmap "찍",		$08, $8f
	charmap "찐",		$08, $90
	charmap "찔",		$08, $91
	charmap "찜",		$08, $92
	charmap "찝",		$08, $93
	charmap "찡",		$08, $94
	charmap "찢",		$08, $95
	charmap "찧",		$08, $96
	charmap "차",		$08, $97
	charmap "착",		$08, $98
	charmap "찬",		$08, $99
	charmap "찮",		$08, $9a
	charmap "찰",		$08, $9b
	charmap "참",		$08, $9c
	charmap "찹",		$08, $9d
	charmap "찻",		$08, $9e
	charmap "찼",		$08, $a1
	charmap "창",		$08, $a2
	charmap "찾",		$08, $a3
	charmap "채",		$08, $a4
	charmap "책",		$08, $a5
	charmap "챈",		$08, $a6
	charmap "챌",		$08, $a7
	charmap "챔",		$08, $a8
	charmap "챕",		$08, $a9
	charmap "챗",		$08, $aa
	charmap "챘",		$08, $ab
	charmap "챙",		$08, $ac
	charmap "챠",		$08, $ad
	charmap "챤",		$08, $ae
	charmap "챦",		$08, $af
	charmap "챨",		$08, $b0
	charmap "챰",		$08, $b1
	charmap "챵",		$08, $b2
	charmap "처",		$08, $b3
	charmap "척",		$08, $b4
	charmap "천",		$08, $b5
	charmap "철",		$08, $b6
	charmap "첨",		$08, $b7
	charmap "첩",		$08, $b8
	charmap "첫",		$08, $b9
	charmap "첬",		$08, $ba
	charmap "청",		$08, $bb
	charmap "체",		$08, $bc
	charmap "첵",		$08, $bd
	charmap "첸",		$08, $be
	charmap "첼",		$08, $bf
	charmap "쳄",		$08, $c0
	charmap "쳅",		$08, $c1
	charmap "쳇",		$08, $c2
	charmap "쳉",		$08, $c3
	charmap "쳐",		$08, $c4
	charmap "쳔",		$08, $c5
	charmap "쳤",		$08, $c6
	charmap "쳬",		$08, $c7
	charmap "쳰",		$08, $c8
	charmap "촁",		$08, $c9
	charmap "초",		$08, $ca
	charmap "촉",		$08, $cb
	charmap "촌",		$08, $cc
	charmap "촐",		$08, $cd
	charmap "촘",		$08, $ce
	charmap "촙",		$08, $cf
	charmap "촛",		$08, $d0
	charmap "총",		$08, $d1
	charmap "촤",		$08, $d2
	charmap "촨",		$08, $d3
	charmap "촬",		$08, $d4
	charmap "촹",		$08, $d5
	charmap "최",		$08, $d6
	charmap "쵠",		$08, $d7
	charmap "쵤",		$08, $d8
	charmap "쵬",		$08, $d9
	charmap "쵭",		$08, $da
	charmap "쵯",		$08, $db
	charmap "쵱",		$08, $dc
	charmap "쵸",		$08, $dd
	charmap "춈",		$08, $de
	charmap "추",		$08, $df
	charmap "축",		$08, $e0
	charmap "춘",		$08, $e1
	charmap "출",		$08, $e2
	charmap "춤",		$08, $e3
	charmap "춥",		$08, $e4
	charmap "춧",		$08, $e5
	charmap "충",		$08, $e6
	charmap "춰",		$08, $e7
	charmap "췄",		$08, $e8
	charmap "췌",		$08, $e9
	charmap "췐",		$08, $ea
	charmap "취",		$08, $eb
	charmap "췬",		$08, $ec
	charmap "췰",		$08, $ed
	charmap "췸",		$08, $ee
	charmap "췹",		$08, $ef
	charmap "췻",		$08, $f0
	charmap "췽",		$08, $f1
	charmap "츄",		$08, $f2
	charmap "츈",		$08, $f3
	charmap "츌",		$08, $f4
	charmap "츔",		$08, $f5
	charmap "츙",		$08, $f6
	charmap "츠",		$08, $f7
	charmap "측",		$08, $f8
	charmap "츤",		$08, $f9
	charmap "츨",		$08, $fa
	charmap "츰",		$08, $fb
	charmap "츱",		$08, $fc
	charmap "츳",		$08, $fd
	charmap "층",		$08, $fe

	charmap "치",		$09, $01
	charmap "칙",		$09, $02
	charmap "친",		$09, $03
	charmap "칟",		$09, $04
	charmap "칠",		$09, $05
	charmap "칡",		$09, $06
	charmap "침",		$09, $07
	charmap "칩",		$09, $08
	charmap "칫",		$09, $09
	charmap "칭",		$09, $0a
	charmap "카",		$09, $0b
	charmap "칵",		$09, $0c
	charmap "칸",		$09, $0d
	charmap "칼",		$09, $0e
	charmap "캄",		$09, $0f
	charmap "캅",		$09, $10
	charmap "캇",		$09, $11
	charmap "캉",		$09, $12
	charmap "캐",		$09, $13
	charmap "캑",		$09, $14
	charmap "캔",		$09, $15
	charmap "캘",		$09, $16
	charmap "캠",		$09, $17
	charmap "캡",		$09, $18
	charmap "캣",		$09, $19
	charmap "캤",		$09, $1a
	charmap "캥",		$09, $1b
	charmap "캬",		$09, $1c
	charmap "캭",		$09, $1d
	charmap "컁",		$09, $1e
	charmap "커",		$09, $1f
	charmap "컥",		$09, $20
	charmap "컨",		$09, $21
	charmap "컫",		$09, $22
	charmap "컬",		$09, $23
	charmap "컴",		$09, $24
	charmap "컵",		$09, $25
	charmap "컷",		$09, $26
	charmap "컸",		$09, $27
	charmap "컹",		$09, $28
	charmap "케",		$09, $29
	charmap "켁",		$09, $2a
	charmap "켄",		$09, $2b
	charmap "켈",		$09, $2c
	charmap "켐",		$09, $2d
	charmap "켑",		$09, $2e
	charmap "켓",		$09, $2f
	charmap "켕",		$09, $30
	charmap "켜",		$09, $31
	charmap "켠",		$09, $32
	charmap "켤",		$09, $33
	charmap "켬",		$09, $34
	charmap "켭",		$09, $35
	charmap "켯",		$09, $36
	charmap "켰",		$09, $37
	charmap "켱",		$09, $38
	charmap "켸",		$09, $39
	charmap "코",		$09, $3a
	charmap "콕",		$09, $3b
	charmap "콘",		$09, $3c
	charmap "콜",		$09, $3d
	charmap "콤",		$09, $3e
	charmap "콥",		$09, $3f
	charmap "콧",		$09, $40
	charmap "콩",		$09, $41
	charmap "콰",		$09, $42
	charmap "콱",		$09, $43
	charmap "콴",		$09, $44
	charmap "콸",		$09, $45
	charmap "쾀",		$09, $46
	charmap "쾅",		$09, $47
	charmap "쾌",		$09, $48
	charmap "쾡",		$09, $49
	charmap "쾨",		$09, $4a
	charmap "쾰",		$09, $4b
	charmap "쿄",		$09, $4c
	charmap "쿠",		$09, $4d
	charmap "쿡",		$09, $4e
	charmap "쿤",		$09, $4f
	charmap "쿨",		$09, $60
	charmap "쿰",		$09, $61
	charmap "쿱",		$09, $62
	charmap "쿳",		$09, $63
	charmap "쿵",		$09, $64
	charmap "쿼",		$09, $65
	charmap "퀀",		$09, $66
	charmap "퀄",		$09, $67
	charmap "퀑",		$09, $68
	charmap "퀘",		$09, $69
	charmap "퀭",		$09, $6a
	charmap "퀴",		$09, $6b
	charmap "퀵",		$09, $6c
	charmap "퀸",		$09, $6d
	charmap "퀼",		$09, $6e
	charmap "큄",		$09, $71
	charmap "큅",		$09, $72
	charmap "큇",		$09, $73
	charmap "큉",		$09, $74
	charmap "큐",		$09, $75
	charmap "큔",		$09, $76
	charmap "큘",		$09, $77
	charmap "큠",		$09, $78
	charmap "크",		$09, $79
	charmap "큭",		$09, $7a
	charmap "큰",		$09, $7b
	charmap "클",		$09, $7c
	charmap "큼",		$09, $7d
	charmap "큽",		$09, $7e
	charmap "킁",		$09, $7f
	charmap "키",		$09, $80
	charmap "킥",		$09, $81
	charmap "킨",		$09, $82
	charmap "킬",		$09, $83
	charmap "킴",		$09, $84
	charmap "킵",		$09, $85
	charmap "킷",		$09, $86
	charmap "킹",		$09, $87
	charmap "타",		$09, $88
	charmap "탁",		$09, $89
	charmap "탄",		$09, $8a
	charmap "탈",		$09, $8b
	charmap "탉",		$09, $8c
	charmap "탐",		$09, $8d
	charmap "탑",		$09, $8e
	charmap "탓",		$09, $8f
	charmap "탔",		$09, $90
	charmap "탕",		$09, $91
	charmap "태",		$09, $92
	charmap "택",		$09, $93
	charmap "탠",		$09, $94
	charmap "탤",		$09, $95
	charmap "탬",		$09, $96
	charmap "탭",		$09, $97
	charmap "탯",		$09, $98
	charmap "탰",		$09, $99
	charmap "탱",		$09, $9a
	charmap "탸",		$09, $9b
	charmap "턍",		$09, $9c
	charmap "터",		$09, $9d
	charmap "턱",		$09, $9e
	charmap "턴",		$09, $9f
	charmap "털",		$09, $a0
	charmap "턺",		$09, $a1
	charmap "텀",		$09, $a2
	charmap "텁",		$09, $a3
	charmap "텃",		$09, $a4
	charmap "텄",		$09, $a5
	charmap "텅",		$09, $a6
	charmap "테",		$09, $a7
	charmap "텍",		$09, $a8
	charmap "텐",		$09, $a9
	charmap "텔",		$09, $aa
	charmap "템",		$09, $ab
	charmap "텝",		$09, $ac
	charmap "텟",		$09, $ad
	charmap "텡",		$09, $ae
	charmap "텨",		$09, $af
	charmap "텬",		$09, $b0
	charmap "텼",		$09, $b1
	charmap "톄",		$09, $b2
	charmap "톈",		$09, $b3
	charmap "토",		$09, $b4
	charmap "톡",		$09, $b5
	charmap "톤",		$09, $b6
	charmap "톨",		$09, $b7
	charmap "톰",		$09, $b8
	charmap "톱",		$09, $b9
	charmap "톳",		$09, $ba
	charmap "통",		$09, $bb
	charmap "톺",		$09, $bc
	charmap "톼",		$09, $bd
	charmap "퇀",		$09, $be
	charmap "퇘",		$09, $bf
	charmap "퇴",		$09, $c0
	charmap "퇸",		$09, $c1
	charmap "툇",		$09, $c2
	charmap "툉",		$09, $c3
	charmap "툐",		$09, $c4
	charmap "투",		$09, $c5
	charmap "툭",		$09, $c6
	charmap "툰",		$09, $c7
	charmap "툴",		$09, $c8
	charmap "툼",		$09, $c9
	charmap "툽",		$09, $ca
	charmap "툿",		$09, $cb
	charmap "퉁",		$09, $cc
	charmap "퉈",		$09, $cd
	charmap "퉜",		$09, $ce
	charmap "퉤",		$09, $d1
	charmap "튀",		$09, $d2
	charmap "튁",		$09, $d3
	charmap "튄",		$09, $d4
	charmap "튈",		$09, $d5
	charmap "튐",		$09, $d6
	charmap "튑",		$09, $d7
	charmap "튕",		$09, $d8
	charmap "튜",		$09, $d9
	charmap "튠",		$09, $da
	charmap "튤",		$09, $db
	charmap "튬",		$09, $dc
	charmap "튱",		$09, $dd
	charmap "트",		$09, $de
	charmap "특",		$09, $df
	charmap "튼",		$09, $e0
	charmap "튿",		$09, $e1
	charmap "틀",		$09, $e2
	charmap "틂",		$09, $e3
	charmap "틈",		$09, $e4
	charmap "틉",		$09, $e5
	charmap "틋",		$09, $e6
	charmap "틔",		$09, $e7
	charmap "틘",		$09, $e8
	charmap "틜",		$09, $e9
	charmap "틤",		$09, $ea
	charmap "틥",		$09, $eb
	charmap "티",		$09, $ec
	charmap "틱",		$09, $ed
	charmap "틴",		$09, $ee
	charmap "틸",		$09, $ef
	charmap "팀",		$09, $f0
	charmap "팁",		$09, $f1
	charmap "팃",		$09, $f2
	charmap "팅",		$09, $f3
	charmap "파",		$09, $f4
	charmap "팍",		$09, $f5
	charmap "팎",		$09, $f6
	charmap "판",		$09, $f7
	charmap "팔",		$09, $f8
	charmap "팖",		$09, $f9
	charmap "팜",		$09, $fa
	charmap "팝",		$09, $fb
	charmap "팟",		$09, $fc
	charmap "팠",		$09, $fd
	charmap "팡",		$09, $fe
	charmap "팥",		$09, $ff

	charmap "패",		$0a, $00
	charmap "팩",		$0a, $01
	charmap "팬",		$0a, $02
	charmap "팰",		$0a, $03
	charmap "팸",		$0a, $04
	charmap "팹",		$0a, $05
	charmap "팻",		$0a, $06
	charmap "팼",		$0a, $07
	charmap "팽",		$0a, $08
	charmap "퍄",		$0a, $09
	charmap "퍅",		$0a, $0a
	charmap "퍼",		$0a, $0b
	charmap "퍽",		$0a, $0c
	charmap "펀",		$0a, $0d
	charmap "펄",		$0a, $0e
	charmap "펌",		$0a, $0f
	charmap "펍",		$0a, $10
	charmap "펏",		$0a, $11
	charmap "펐",		$0a, $12
	charmap "펑",		$0a, $13
	charmap "페",		$0a, $14
	charmap "펙",		$0a, $15
	charmap "펜",		$0a, $16
	charmap "펠",		$0a, $17
	charmap "펨",		$0a, $18
	charmap "펩",		$0a, $19
	charmap "펫",		$0a, $1a
	charmap "펭",		$0a, $1b
	charmap "펴",		$0a, $1c
	charmap "편",		$0a, $1d
	charmap "펼",		$0a, $1e
	charmap "폄",		$0a, $1f
	charmap "폅",		$0a, $20
	charmap "폈",		$0a, $21
	charmap "평",		$0a, $22
	charmap "폐",		$0a, $23
	charmap "폘",		$0a, $24
	charmap "폡",		$0a, $25
	charmap "폣",		$0a, $26
	charmap "포",		$0a, $27
	charmap "폭",		$0a, $28
	charmap "폰",		$0a, $29
	charmap "폴",		$0a, $2a
	charmap "폼",		$0a, $2b
	charmap "폽",		$0a, $2c
	charmap "폿",		$0a, $2d
	charmap "퐁",		$0a, $2e
	charmap "퐈",		$0a, $31
	charmap "퐝",		$0a, $32
	charmap "푀",		$0a, $33
	charmap "푄",		$0a, $34
	charmap "표",		$0a, $35
	charmap "푠",		$0a, $36
	charmap "푤",		$0a, $37
	charmap "푭",		$0a, $38
	charmap "푯",		$0a, $39
	charmap "푸",		$0a, $3a
	charmap "푹",		$0a, $3b
	charmap "푼",		$0a, $3c
	charmap "푿",		$0a, $3d
	charmap "풀",		$0a, $3e
	charmap "풂",		$0a, $3f
	charmap "품",		$0a, $40
	charmap "풉",		$0a, $41
	charmap "풋",		$0a, $42
	charmap "풍",		$0a, $43
	charmap "풔",		$0a, $44
	charmap "풩",		$0a, $45
	charmap "퓌",		$0a, $46
	charmap "퓐",		$0a, $47
	charmap "퓔",		$0a, $48
	charmap "퓜",		$0a, $49
	charmap "퓟",		$0a, $4a
	charmap "퓨",		$0a, $4b
	charmap "퓬",		$0a, $4c
	charmap "퓰",		$0a, $4d
	charmap "퓸",		$0a, $4e
	charmap "퓻",		$0a, $4f
	charmap "퓽",		$0a, $60
	charmap "프",		$0a, $61
	charmap "픈",		$0a, $62
	charmap "플",		$0a, $63
	charmap "픔",		$0a, $64
	charmap "픕",		$0a, $65
	charmap "픗",		$0a, $66
	charmap "피",		$0a, $67
	charmap "픽",		$0a, $68
	charmap "핀",		$0a, $69
	charmap "필",		$0a, $6a
	charmap "핌",		$0a, $6b
	charmap "핍",		$0a, $6c
	charmap "핏",		$0a, $6d
	charmap "핑",		$0a, $6e
	charmap "하",		$0a, $6f
	charmap "학",		$0a, $70
	charmap "한",		$0a, $71
	charmap "할",		$0a, $72
	charmap "핥",		$0a, $73
	charmap "함",		$0a, $74
	charmap "합",		$0a, $75
	charmap "핫",		$0a, $76
	charmap "항",		$0a, $77
	charmap "해",		$0a, $78
	charmap "핵",		$0a, $79
	charmap "핸",		$0a, $7a
	charmap "핼",		$0a, $7b
	charmap "햄",		$0a, $7c
	charmap "햅",		$0a, $7d
	charmap "햇",		$0a, $7e
	charmap "했",		$0a, $7f
	charmap "행",		$0a, $80
	charmap "햐",		$0a, $81
	charmap "향",		$0a, $82
	charmap "허",		$0a, $83
	charmap "헉",		$0a, $84
	charmap "헌",		$0a, $85
	charmap "헐",		$0a, $86
	charmap "헒",		$0a, $87
	charmap "험",		$0a, $88
	charmap "헙",		$0a, $89
	charmap "헛",		$0a, $8a
	charmap "헝",		$0a, $8b
	charmap "헤",		$0a, $8c
	charmap "헥",		$0a, $8d
	charmap "헨",		$0a, $8e
	charmap "헬",		$0a, $8f
	charmap "헴",		$0a, $90
	charmap "헵",		$0a, $91
	charmap "헷",		$0a, $92
	charmap "헹",		$0a, $93
	charmap "혀",		$0a, $94
	charmap "혁",		$0a, $95
	charmap "현",		$0a, $96
	charmap "혈",		$0a, $97
	charmap "혐",		$0a, $98
	charmap "협",		$0a, $99
	charmap "혓",		$0a, $9a
	charmap "혔",		$0a, $9b
	charmap "형",		$0a, $9c
	charmap "혜",		$0a, $9d
	charmap "혠",		$0a, $9e
	charmap "혤",		$0a, $a1
	charmap "혭",		$0a, $a2
	charmap "호",		$0a, $a3
	charmap "혹",		$0a, $a4
	charmap "혼",		$0a, $a5
	charmap "홀",		$0a, $a6
	charmap "홅",		$0a, $a7
	charmap "홈",		$0a, $a8
	charmap "홉",		$0a, $a9
	charmap "홋",		$0a, $aa
	charmap "홍",		$0a, $ab
	charmap "홑",		$0a, $ac
	charmap "화",		$0a, $ad
	charmap "확",		$0a, $ae
	charmap "환",		$0a, $af
	charmap "활",		$0a, $b0
	charmap "홧",		$0a, $b1
	charmap "황",		$0a, $b2
	charmap "홰",		$0a, $b3
	charmap "홱",		$0a, $b4
	charmap "홴",		$0a, $b5
	charmap "횃",		$0a, $b6
	charmap "횅",		$0a, $b7
	charmap "회",		$0a, $b8
	charmap "획",		$0a, $b9
	charmap "횐",		$0a, $ba
	charmap "횔",		$0a, $bb
	charmap "횝",		$0a, $bc
	charmap "횟",		$0a, $bd
	charmap "횡",		$0a, $be
	charmap "효",		$0a, $bf
	charmap "횬",		$0a, $c0
	charmap "횰",		$0a, $c1
	charmap "횹",		$0a, $c2
	charmap "횻",		$0a, $c3
	charmap "후",		$0a, $c4
	charmap "훅",		$0a, $c5
	charmap "훈",		$0a, $c6
	charmap "훌",		$0a, $c7
	charmap "훑",		$0a, $c8
	charmap "훔",		$0a, $c9
	charmap "훗",		$0a, $ca
	charmap "훙",		$0a, $cb
	charmap "훠",		$0a, $cc
	charmap "훤",		$0a, $cd
	charmap "훨",		$0a, $ce
	charmap "훰",		$0a, $cf
	charmap "훵",		$0a, $d0
	charmap "훼",		$0a, $d1
	charmap "훽",		$0a, $d2
	charmap "휀",		$0a, $d3
	charmap "휄",		$0a, $d4
	charmap "휑",		$0a, $d5
	charmap "휘",		$0a, $d6
	charmap "휙",		$0a, $d7
	charmap "휜",		$0a, $d8
	charmap "휠",		$0a, $d9
	charmap "휨",		$0a, $da
	charmap "휩",		$0a, $db
	charmap "휫",		$0a, $dc
	charmap "휭",		$0a, $dd
	charmap "휴",		$0a, $de
	charmap "휵",		$0a, $df
	charmap "휸",		$0a, $e0
	charmap "휼",		$0a, $e1
	charmap "흄",		$0a, $e2
	charmap "흇",		$0a, $e3
	charmap "흉",		$0a, $e4
	charmap "흐",		$0a, $e5
	charmap "흑",		$0a, $e6
	charmap "흔",		$0a, $e7
	charmap "흖",		$0a, $e8
	charmap "흗",		$0a, $e9
	charmap "흘",		$0a, $ea
	charmap "흙",		$0a, $eb
	charmap "흠",		$0a, $ec
	charmap "흡",		$0a, $ed
	charmap "흣",		$0a, $ee
	charmap "흥",		$0a, $ef
	charmap "흩",		$0a, $f0
	charmap "희",		$0a, $f1
	charmap "흰",		$0a, $f2
	charmap "흴",		$0a, $f3
	charmap "흼",		$0a, $f4
	charmap "흽",		$0a, $f5
	charmap "힁",		$0a, $f6
	charmap "히",		$0a, $f7
	charmap "힉",		$0a, $f8
	charmap "힌",		$0a, $f9
	charmap "힐",		$0a, $fa
	charmap "힘",		$0a, $fb
	charmap "힙",		$0a, $fc
	charmap "힛",		$0a, $fd
	charmap "힝",		$0a, $fe

	charmap "ㄱ",		$0b, $00
	charmap "ㄴ",		$0b, $01
	charmap "ㄷ",		$0b, $02
	charmap "ㄹ",		$0b, $03
	charmap "ㅁ",		$0b, $04
	charmap "ㅂ",		$0b, $05
	charmap "ㅅ",		$0b, $06
	charmap "ㅇ",		$0b, $07
	charmap "ㅈ",		$0b, $08
	charmap "ㅊ",		$0b, $09
	charmap "ㅋ",		$0b, $0a
	charmap "ㅌ",		$0b, $0b
	charmap "ㅍ",		$0b, $0c
	charmap "ㅎ",		$0b, $0d
	charmap "ㄲ",		$0b, $0e
	charmap "ㄸ",		$0b, $0f
	charmap "ㅃ",		$0b, $10
	charmap "ㅆ",		$0b, $11
	charmap "ㅉ",		$0b, $12
	charmap "ㅏ",		$0b, $20
	charmap "ㅑ",		$0b, $21
	charmap "ㅓ",		$0b, $22
	charmap "ㅕ",		$0b, $23
	charmap "ㅗ",		$0b, $24
	charmap "ㅛ",		$0b, $25
	charmap "ㅜ",		$0b, $26
	charmap "ㅠ",		$0b, $27
	charmap "ㅡ",		$0b, $28
	charmap "ㅣ",		$0b, $29
	charmap "ㅐ",		$0b, $2a
	charmap "ㅒ",		$0b, $2b
	charmap "ㅔ",		$0b, $2c
	charmap "ㅖ",		$0b, $2d
	charmap "ㅘ",		$0b, $2e
	charmap "ㅙ",		$0b, $2f
	charmap "ㅚ",		$0b, $30
	charmap "ㅝ",		$0b, $31
	charmap "ㅞ",		$0b, $32
	charmap "ㅟ",		$0b, $33
	charmap "ㅢ",		$0b, $34
	charmap "_",		$0b, $3e
	charmap "「",		$0b, $60
	charmap "」",		$0b, $61
	charmap "『",		$0b, $62
	charmap "』",		$0b, $63
;	charmap "(",		$0b, $64
;	charmap ")",		$0b, $65
;	charmap "!",		$0b, $66
;	charmap "?",		$0b, $67
;	charmap "-",		$0b, $68
;	charmap "~",		$0b, $69
;	charmap "...",		$0b, $6a
;	charmap ",",		$0b, $6b
;	charmap ".",		$0b, $6b
;	charmap "0",		$0b, $f0
;	charmap "1",		$0b, $f1
;	charmap "2",		$0b, $f2
;	charmap "3",		$0b, $f3
;	charmap "4",		$0b, $f4
;	charmap "5",		$0b, $f5
;	charmap "6",		$0b, $f6
;	charmap "7",		$0b, $f7
;	charmap "8",		$0b, $f8
;	charmap "9",		$0b, $f9
	charmap " ",		$0b, $fa

; Actual characters (from gfx/font/font.png)

	charmap "A",         $80
	charmap "B",         $81
	charmap "C",         $82
	charmap "D",         $83
	charmap "E",         $84
	charmap "F",         $85
	charmap "G",         $86
	charmap "H",         $87
	charmap "I",         $88
	charmap "J",         $89
	charmap "K",         $8a
	charmap "L",         $8b
	charmap "M",         $8c
	charmap "N",         $8d
	charmap "O",         $8e
	charmap "P",         $8f
	charmap "Q",         $90
	charmap "R",         $91
	charmap "S",         $92
	charmap "T",         $93
	charmap "U",         $94
	charmap "V",         $95
	charmap "W",         $96
	charmap "X",         $97
	charmap "Y",         $98
	charmap "Z",         $99

	charmap "(",         $9a
	charmap ")",         $9b
	charmap ":",         $9c
	charmap ";",         $9d
	charmap "[",         $9e
	charmap "]",         $9f

	charmap "a",         $a0
	charmap "b",         $a1
	charmap "c",         $a2
	charmap "d",         $a3
	charmap "e",         $a4
	charmap "f",         $a5
	charmap "g",         $a6
	charmap "h",         $a7
	charmap "i",         $a8
	charmap "j",         $a9
	charmap "k",         $aa
	charmap "l",         $ab
	charmap "m",         $ac
	charmap "n",         $ad
	charmap "o",         $ae
	charmap "p",         $af
	charmap "q",         $b0
	charmap "r",         $b1
	charmap "s",         $b2
	charmap "t",         $b3
	charmap "u",         $b4
	charmap "v",         $b5
	charmap "w",         $b6
	charmap "x",         $b7
	charmap "y",         $b8
	charmap "z",         $b9

	charmap "Ä",         $c0
	charmap "Ö",         $c1
	charmap "Ü",         $c2
	charmap "ä",         $c3
	charmap "ö",         $c4
	charmap "ü",         $c5

	charmap "'d",        $d0
	charmap "'l",        $d1
	charmap "'m",        $d2
	charmap "'r",        $d3
	charmap "'s",        $d4
	charmap "'t",        $d5
	charmap "'v",        $d6

	charmap "←",         $df
	charmap "'",         $e0
	charmap "<PK>",      $e1
	charmap "<MN>",      $e2
	charmap "-",         $e3

	charmap "?",         $e6
	charmap "!",         $e7
	charmap ".",         $e8
	charmap "&",         $e9

	charmap "é",         $ea
	charmap "→",         $eb
	charmap "▷",         $ec
	charmap "▶",         $ed
	charmap "▼",         $ee
	charmap "♂",         $ef
	charmap "¥",         $f0 ; Poké Dollar sign
	charmap "×",         $f1
	charmap "<DOT>",     $f2 ; decimal point; same as "." in English
	charmap "/",         $f3
	charmap ",",         $f4
	charmap "♀",         $f5

	charmap "0",         $f6
	charmap "1",         $f7
	charmap "2",         $f8
	charmap "3",         $f9
	charmap "4",         $fa
	charmap "5",         $fb
	charmap "6",         $fc
	charmap "7",         $fd
	charmap "8",         $fe
	charmap "9",         $ff

; Japanese control characters (see home/text.asm)

	charmap "<JP_14>",   $14 ; "ナﾞ" (ungrammatical)
	charmap "<JP_18>",   $18 ; "ノ゛" (ungrammatical)
	charmap "<NI>",      $1d ; "に　"
	charmap "<TTE>",     $1e ; "って"
	charmap "<WO>",      $1f ; "を　"
	charmap "<TA!>",     $22 ; "た！"
	charmap "<KOUGEKI>", $23 ; "こうげき"
	charmap "<WA>",      $24 ; "は　"
	charmap "<NO>",      $25 ; "の　"
	charmap "<ROUTE>",   $35 ; "ばん　どうろ"
	charmap "<WATASHI>", $36 ; "わたし"
	charmap "<KOKO_WA>", $37 ; "ここは"
	charmap "<GA>",      $4a ; "が　"

; Japanese kana, for those bits of text that were not translated to English

	charmap "ガ", $05
	charmap "ギ", $06
	charmap "グ", $07
	charmap "ゲ", $08
	charmap "ゴ", $09
	charmap "ザ", $0a
	charmap "ジ", $0b
	charmap "ズ", $0c
	charmap "ゼ", $0d
	charmap "ゾ", $0e
	charmap "ダ", $0f
	charmap "ヂ", $10
	charmap "ヅ", $11
	charmap "デ", $12
	charmap "ド", $13

	charmap "バ", $19
	charmap "ビ", $1a
	charmap "ブ", $1b
	charmap "ボ", $1c

	charmap "が", $26
	charmap "ぎ", $27
	charmap "ぐ", $28
	charmap "げ", $29
	charmap "ご", $2a
	charmap "ざ", $2b
	charmap "じ", $2c
	charmap "ず", $2d
	charmap "ぜ", $2e
	charmap "ぞ", $2f
	charmap "だ", $30
	charmap "ぢ", $31
	charmap "づ", $32
	charmap "で", $33
	charmap "ど", $34

	charmap "ば", $3a
	charmap "び", $3b
	charmap "ぶ", $3c
	charmap "べ", $3d
	charmap "ぼ", $3e

	charmap "パ", $40
	charmap "ピ", $41
	charmap "プ", $42
	charmap "ポ", $43
	charmap "ぱ", $44
	charmap "ぴ", $45
	charmap "ぷ", $46
	charmap "ぺ", $47
	charmap "ぽ", $48

	charmap "「", $70
	charmap "」", $71
	charmap "』", $73
	charmap "・", $74
	charmap "⋯", $75

;	charmap "　", $7f

	charmap "ア", $80
	charmap "イ", $81
	charmap "ウ", $82
	charmap "エ", $83
	charmap "オ", $84
	charmap "カ", $85
	charmap "キ", $86
	charmap "ク", $87
	charmap "ケ", $88
	charmap "コ", $89
	charmap "サ", $8a
	charmap "シ", $8b
	charmap "ス", $8c
	charmap "セ", $8d
	charmap "ソ", $8e
	charmap "タ", $8f
	charmap "チ", $90
	charmap "ツ", $91
	charmap "テ", $92
	charmap "ト", $93
	charmap "ナ", $94
	charmap "ニ", $95
	charmap "ヌ", $96
	charmap "ネ", $97
	charmap "ノ", $98
	charmap "ハ", $99
	charmap "ヒ", $9a
	charmap "フ", $9b
	charmap "ホ", $9c
	charmap "マ", $9d
	charmap "ミ", $9e
	charmap "ム", $9f
	charmap "メ", $a0
	charmap "モ", $a1
	charmap "ヤ", $a2
	charmap "ユ", $a3
	charmap "ヨ", $a4
	charmap "ラ", $a5
	charmap "ル", $a6
	charmap "レ", $a7
	charmap "ロ", $a8
	charmap "ワ", $a9
	charmap "ヲ", $aa
	charmap "ン", $ab
	charmap "ッ", $ac
	charmap "ャ", $ad
	charmap "ュ", $ae
	charmap "ョ", $af
	charmap "ィ", $b0

	charmap "あ", $b1
	charmap "い", $b2
	charmap "う", $b3
	charmap "え", $b4
	charmap "お", $b5
	charmap "か", $b6
	charmap "き", $b7
	charmap "く", $b8
	charmap "け", $b9
	charmap "こ", $ba
	charmap "さ", $bb
	charmap "し", $bc
	charmap "す", $bd
	charmap "せ", $be
	charmap "そ", $bf
	charmap "た", $c0
	charmap "ち", $c1
	charmap "つ", $c2
	charmap "て", $c3
	charmap "と", $c4
	charmap "な", $c5
	charmap "に", $c6
	charmap "ぬ", $c7
	charmap "ね", $c8
	charmap "の", $c9
	charmap "は", $ca
	charmap "ひ", $cb
	charmap "ふ", $cc
	charmap "へ", $cd
	charmap "ほ", $ce
	charmap "ま", $cf
	charmap "み", $d0
	charmap "む", $d1
	charmap "め", $d2
	charmap "も", $d3
	charmap "や", $d4
	charmap "ゆ", $d5
	charmap "よ", $d6
	charmap "ら", $d7
	charmap "り", $d8
	charmap "る", $d9
	charmap "れ", $da
	charmap "ろ", $db
	charmap "わ", $dc
	charmap "を", $dd
	charmap "ん", $de
	charmap "っ", $df
	charmap "ゃ", $e0
	charmap "ゅ", $e1
	charmap "ょ", $e2

	charmap "ー", $e3
	charmap "ﾟ", $e4
	charmap "ﾞ", $e5

	charmap "？", $e6
	charmap "！", $e7
	charmap "。", $e8

	charmap "ァ", $e9
	charmap "ゥ", $ea
	charmap "ェ", $eb

	charmap "円", $f0

	charmap "．", $f2
	charmap "／", $f3

	charmap "ォ", $f4

	charmap "０", $f6
	charmap "１", $f7
	charmap "２", $f8
	charmap "３", $f9
	charmap "４", $fa
	charmap "５", $fb
	charmap "６", $fc
	charmap "７", $fd
	charmap "８", $fe
	charmap "９", $ff
