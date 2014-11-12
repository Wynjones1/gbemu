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

