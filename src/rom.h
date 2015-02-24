#pragma once


enum CART_TYPE
{
	CART_TYPE_ROM_ONLY                = 0x00,
	CART_TYPE_MBC1                    = 0x01,
	CART_TYPE_MBC1_RAM                = 0x02,
	CART_TYPE_MBC1_RAM_BATTERY        = 0x03,
	CART_TYPE_MBC2                    = 0x05,
	CART_TYPE_MBC2_BATTERY            = 0x06,
	CART_TYPE_ROM_RAM                 = 0x08,
	CART_TYPE_ROM_RAM_BATTERY         = 0x09,
	CART_TYPE_MMM01                   = 0x0b,
	CART_TYPE_MMM01_RAM               = 0x0c,
	CART_TYPE_MMM01_RAM_BATTERY       = 0x0d,
	CART_TYPE_MBC3_TIMER_BATTERY      = 0x0f,
	CART_TYPE_MBC3_TIMER_RAM_BATTERY  = 0x10,
	CART_TYPE_MBC3                    = 0x11,
	CART_TYPE_MBC3_RAM                = 0x12,
	CART_TYPE_MBC3_RAM_BATTERY        = 0x13,
	CART_TYPE_MBC4                    = 0x15,
	CART_TYPE_MBC4_RAM                = 0x16,
	CART_TYPE_MBC4_RAM_BATTERY        = 0x17,
	CART_TYPE_MBC5                    = 0x19,
	CART_TYPE_MBC5_RAM                = 0x1a,
	CART_TYPE_MBC5_RAM_BATTERY        = 0x1b,
	CART_TYPE_MBC5_RUMBLE             = 0x1c,
	CART_TYPE_MBC5_RUMBLE_RAM         = 0x1d,
	CART_TYPE_MBC5_RUMBLE_RAM_BATTERY = 0x1e,
	CART_TYPE_POCKET_CAMERA           = 0xfc,
	CART_TYPE_BANDAI_TAMA5            = 0xfd,
	CART_TYPE_HUC3                    = 0xfe,
	CART_TYPE_HUC1_RAM_BATTERY        = 0xff
};

#define IS_MBC(x, y) (x->cart_type == y)
#define IS_MBC1(x) (IS_MBC(x, CART_TYPE_MBC1)     ||\
                    IS_MBC(x, CART_TYPE_MBC1_RAM) ||\
                    IS_MBC(x, CART_TYPE_MBC1_RAM_BATTERY))

#define IS_MBC3(x) IS_MBC(x, CART_TYPE_MBC3_TIMER_BATTERY)     ||\
                   IS_MBC(x, CART_TYPE_MBC3_TIMER_RAM_BATTERY) ||\
                   IS_MBC(x, CART_TYPE_MBC3)                   ||\
                   IS_MBC(x, CART_TYPE_MBC3_RAM)               ||\
                   IS_MBC(x, CART_TYPE_MBC3_RAM_BATTERY)
#undef X

enum ROM_SIZE
{
    ROM_SIZE_32K   = 0x0,
    ROM_SIZE_64K   = 0x1,
    ROM_SIZE_128K  = 0x2,
    ROM_SIZE_256K  = 0x3,
    ROM_SIZE_512K  = 0x4,
    ROM_SIZE_1024K = 0x5,
    ROM_SIZE_2048K = 0x6,
    ROM_SIZE_4097K = 0x7,
};

enum SAVE_RAM_SIZE
{
    SAVE_RAM_SIZE_0K  = 0x0,
    SAVE_RAM_SIZE_2K  = 0x1,
    SAVE_RAM_SIZE_8K  = 0x2,
    SAVE_RAM_SIZE_32K = 0x3,
};

enum COUNTRY_CODE
{
    COUNTRY_CODE_JAPAN = 0,
    COUNTRY_CODE_OTHER = 1
};

typedef struct rom
{
	char title[16];
	char manufacturer_code[5];
	char cgb_flag;
	char sgb_flag;
	char licensee_code[3];
	char cart_type;
	char rom_size;
	char ram_size;
}rom_t;


/*
* Read in the ROM data and display info.
*/
rom_t rom_read(const char *filename);




enum LICENSEE_CODE
{
    LICENSEE_CODE_ABSOLUTE              = 0x50,
    LICENSEE_CODE_ACCLAIM_0             = 0x51,
    LICENSEE_CODE_ACCLAIM_1             = 0xB0,
    LICENSEE_CODE_ACCOLADE              = 0x79,
    LICENSEE_CODE_ACTIVISION            = 0x52,
    LICENSEE_CODE_ALTRON                = 0xDF,
    LICENSEE_CODE_AMERICAN_SAMMY        = 0x53,
    LICENSEE_CODE_ANGEL_0               = 0x46,
    LICENSEE_CODE_ANGEL_1               = 0xCF,
    LICENSEE_CODE_APE                   = 0x8E,
    LICENSEE_CODE_ARC                   = 0x99,
    LICENSEE_CODE_ASCII_OR_NEXOFT       = 0xB1,
    LICENSEE_CODE_ASK_KODANSHA          = 0xD4,
    LICENSEE_CODE_ASMIK                 = 0xE8,
    LICENSEE_CODE_ATHENA                = 0xE7,
    LICENSEE_CODE_ATLUS_0               = 0x42,
    LICENSEE_CODE_ATLUS_1               = 0xEB,
    LICENSEE_CODE_A_WAVE                = 0xF0,
    LICENSEE_CODE_BANDAI_0              = 0x32,
    LICENSEE_CODE_BANDAI_1              = 0xA2,
    LICENSEE_CODE_BANDAI_2              = 0xB2,
    LICENSEE_CODE_BANPRESTO_0           = 0xD9,
    LICENSEE_CODE_BANPRESTO_1           = 0x39,
    LICENSEE_CODE_BANPRESTO_2           = 0x9D,
    LICENSEE_CODE_BRODERBUND_0          = 0xAA,
    LICENSEE_CODE_BRODERBUND_1          = 0x72,
    LICENSEE_CODE_BULLET_PROOF_SOFTWARE = 0x8B,
    LICENSEE_CODE_CAPCOM_0              = 0x08,
    LICENSEE_CODE_CAPCOM_1              = 0x38,
    LICENSEE_CODE_CHUN_SOFT             = 0x91,
    LICENSEE_CODE_CLARY                 = 0x1D,
    LICENSEE_CODE_COCONUTS              = 0x0B,
    LICENSEE_CODE_COPYA_SYSTEMS         = 0xD7,
    LICENSEE_CODE_CULTURE_BRAIN_O       = 0xBA,
    LICENSEE_CODE_DATA_EAST             = 0xC5,
    LICENSEE_CODE_ELECTRONIC_ARTS_0     = 0x13,
    LICENSEE_CODE_ELECTRONIC_ARTS_1     = 0x69,
    LICENSEE_CODE_ELECTRO_BRAIN         = 0x6F,
    LICENSEE_CODE_ELITE_SYSTEMS_0       = 0x6E,
    LICENSEE_CODE_ELITE_SYSTEMS_1       = 0x0C,
    LICENSEE_CODE_ENIX                  = 0xB4,
    LICENSEE_CODE_ENTERTAINMENT_I       = 0x3C,
    LICENSEE_CODE_EPIC_SONY_RECORDS     = 0xEC,
    LICENSEE_CODE_EPOCH                 = 0xE5,
    LICENSEE_CODE_EXTREME_ENTERTAINMENT = 0xF3,
    LICENSEE_CODE_GBC                   = 0x33,
    LICENSEE_CODE_GAMETEK               = 0x54,
    LICENSEE_CODE_GREMLIN               = 0x3E,
    LICENSEE_CODE_HAL                   = 0xB6,
    LICENSEE_CODE_HECTOR                = 0x35,
    LICENSEE_CODE_HORI_ELECTRIC         = 0xA1,
    LICENSEE_CODE_HOT_B                 = 0x09,
    LICENSEE_CODE_HUDSONSOFT            = 0x18,
    LICENSEE_CODE_HUMAN                 = 0xDE,
    LICENSEE_CODE_IGS                   = 0xEE,
    LICENSEE_CODE_IMAGINEER             = 0x9C,
    LICENSEE_CODE_INFOGRAMES_0          = 0x30,
    LICENSEE_CODE_INFOGRAMES_1          = 0x70,
    LICENSEE_CODE_INTERPLAY             = 0x71,
    LICENSEE_CODE_IREM                  = 0x49,
    LICENSEE_CODE_ITC_ENTERTAINMENT     = 0x19,
    LICENSEE_CODE_I_MAX                 = 0x8F,
    LICENSEE_CODE_JALECO_0              = 0x0A,
    LICENSEE_CODE_JALECO_1              = 0xE0,
    LICENSEE_CODE_KANEKO                = 0x97,
    LICENSEE_CODE_KAWADA                = 0xA6,
    LICENSEE_CODE_KEMCO_0               = 0x7F,
    LICENSEE_CODE_KEMCO_1               = 0xC2,
    LICENSEE_CODE_KING_RECORDS          = 0xEA,
    LICENSEE_CODE_KOEI                  = 0xC8,
    LICENSEE_CODE_KONAMI_0              = 0x34,
    LICENSEE_CODE_KONAMI_1              = 0xA4,
    LICENSEE_CODE_KOTOBUKI_SYSTEMS      = 0x28,
    LICENSEE_CODE_LJN_0                 = 0xDB,
    LICENSEE_CODE_LJN_1                 = 0x56,
    LICENSEE_CODE_LJN_2                 = 0xFF,
    LICENSEE_CODE_LOZC                  = 0x83,
    LICENSEE_CODE_MALIBU_0              = 0x44,
    LICENSEE_CODE_MALIBU_1              = 0x4D,
    LICENSEE_CODE_MATCHBOX              = 0x57,
    LICENSEE_CODE_MELDAC                = 0xCD,
    LICENSEE_CODE_MICROPROSE            = 0x7C,
    LICENSEE_CODE_MILTON_BRADLEY        = 0x59,
    LICENSEE_CODE_MINDSCAPE             = 0x5A,
    LICENSEE_CODE_MISAWA_ENTERTAINMENT  = 0x80,
    LICENSEE_CODE_NAMCO                 = 0xAF,
    LICENSEE_CODE_NATSUME               = 0xE9,
    LICENSEE_CODE_NAXAT_SOFT_0          = 0x5C,
    LICENSEE_CODE_NAXAT_SOFT_1          = 0xD6,
    LICENSEE_CODE_NCS                   = 0xDD,
    LICENSEE_CODE_NIHON_BUSSAN          = 0x9A,
    LICENSEE_CODE_NINTENDO_0            = 0x01,
    LICENSEE_CODE_NINTENDO_1            = 0x31,
    LICENSEE_CODE_NONE                  = 0x00,
    LICENSEE_CODE_NOVA                  = 0x9F,
    LICENSEE_CODE_OCEAN                 = 0x67,
    LICENSEE_CODE_PARK_PLACE            = 0x55,
    LICENSEE_CODE_PCM_COMPLETE          = 0x24,
    LICENSEE_CODE_PONY_CANYON           = 0xB9,
    LICENSEE_CODE_PONY_CANYON_OR        = 0xCE,
    LICENSEE_CODE_QUEST                 = 0xD2,
    LICENSEE_CODE_ROMSTAR               = 0x5B,
    LICENSEE_CODE_SAMMY                 = 0xBF,
    LICENSEE_CODE_SAN_X                 = 0x25,
    LICENSEE_CODE_SCULPTERED_SOFT       = 0x73,
    LICENSEE_CODE_SEE_ABOVE             = 0xBC,
    LICENSEE_CODE_SETA                  = 0x29,
    LICENSEE_CODE_SIGMA_ENTERPRISES     = 0xD3,
    LICENSEE_CODE_SNK                   = 0xB7,
    LICENSEE_CODE_SOFEL                 = 0xD1,
    LICENSEE_CODE_SONY_IMAGESOFT        = 0xBD,
    LICENSEE_CODE_SPECTRUM_HOLOBY       = 0x47,
    LICENSEE_CODE_SQUARESOFT            = 0xC3,
    LICENSEE_CODE_SUNSOFT               = 0xBB,
    LICENSEE_CODE_TAITO_0               = 0xD0,
    LICENSEE_CODE_TAITO_1               = 0xC0,
    LICENSEE_CODE_TAKARA                = 0xA7,
    LICENSEE_CODE_TECHNOS_JAPAN         = 0xA9,
    LICENSEE_CODE_TECMO                 = 0x9B,
    LICENSEE_CODE_THE_SALES_CURVE       = 0x75,
    LICENSEE_CODE_THQ                   = 0x78,
    LICENSEE_CODE_TITUS                 = 0x60,
    LICENSEE_CODE_TOEI_ANIMATION        = 0xAC,
    LICENSEE_CODE_TOHO                  = 0xAD,
    LICENSEE_CODE_TOKUMA_SHOTEN_I_0     = 0x86,
    LICENSEE_CODE_TOKUMA_SHOTEN_I_1     = 0xC4,
    LICENSEE_CODE_TOMY                  = 0xDA,
    LICENSEE_CODE_TONKIN_HOUSE          = 0xC6,
    LICENSEE_CODE_TOWACHIKI             = 0xE1,
    LICENSEE_CODE_TRADEWEST             = 0x5D,
    LICENSEE_CODE_TRIFFIX_ENTERTAINMENT = 0x7A,
    LICENSEE_CODE_TSUBURAVA             = 0x93,
    LICENSEE_CODE_UBI_SOFT              = 0x41,
    LICENSEE_CODE_UFL                   = 0xC9,
    LICENSEE_CODE_ULTRA                 = 0xCA,
    LICENSEE_CODE_USE                   = 0xCC,
    LICENSEE_CODE_UUTAKA                = 0xE2,
    LICENSEE_CODE_U_S_GOLD              = 0x4F,
    LICENSEE_CODE_VAP                   = 0xCB,
    LICENSEE_CODE_VARIE_0               = 0x95,
    LICENSEE_CODE_VARIE_1               = 0xE3,
    LICENSEE_CODE_VIC_TOKAI             = 0x8C,
    LICENSEE_CODE_VIDEO_SYSTEM          = 0x92,
    LICENSEE_CODE_VIRGIN_0              = 0x1F,
    LICENSEE_CODE_VIRGIN_1              = 0x4A,
    LICENSEE_CODE_VIRGIN_2              = 0x61,
    LICENSEE_CODE_YANOMAN               = 0x1A,
    LICENSEE_CODE_YONEZAWAS_PAL         = 0x96,
};
