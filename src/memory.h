#pragma once
#include "types.h"

typedef struct memory memory_t;

memory_t *memory_init(const char *boot, const char *rom);
void      memory_delete(memory_t *mem);
reg_t     memory_load8(memory_t *mem, reg16_t addr);
void      memory_store8(memory_t *mem, reg16_t addr, reg_t data);
reg16_t   memory_load16(memory_t *mem, reg16_t addr);
void      memory_store16(memory_t *mem, reg16_t addr, reg16_t data);
void      memory_load_rom(memory_t *mem, const char *filename);

typedef struct memory
{
	reg_t *bank_0;               // 0x0000 -> 0x3fff
	reg_t *bank_n;               // 0x4000 -> 0x7fff
	reg_t  video_ram[0x2000];    // 0x8000 -> 0x9fff
	reg_t *external_ram;         // 0xa000 -> 0xbfff
	reg_t working_ram_0[0x1000]; // 0xc000 -> 0xcfff
	reg_t working_ram_1[0x1000]; // 0xd000 -> 0xdfff //CGB: Should be switchable.
	reg_t *echo       ;          // 0xe000 -> 0xfdff
	reg_t OAM[0x1e00];           // 0xfe00 -> 0xfe9f
	//unused                     // 0xfea0 -> 0xfeff
	reg_t io_registers[0x80];    // 0xff00 -> 0xff7f
	reg_t stack[0x7f];           // 0xff80 -> 0xfffe
	reg_t interrupt_enable;      // 0xffff

	//Boot Rom.
	int boot_locked;
	reg_t boot[0x100];
	//Banking information and game rom data.
	union
	{
		struct
		{
			char lsb : 5;
			char msb : 2;
		};
		char current_bank;
	};
	int ram_size;
	int cart_type;
	int rom_size;
	int ram_enabled;
	int rom_ram_mode;
	//IO Status registers.
	struct
	{
		uint8_t bg_display     : 1;
		uint8_t obj_enable     : 1;
		uint8_t obj_size       : 1;
		uint8_t map_select     : 1;
		uint8_t tile_select    : 1;
		uint8_t window_display : 1;
		uint8_t window_map     : 1;
		uint8_t enabled        : 1;
	}lcdc;
	uint8_t joypad;
	//Video Registers.
	uint8_t stat;
	uint8_t scy;
	uint8_t scx;
	uint8_t ly;
	uint8_t lyc;
	uint8_t dma;
	uint8_t bgp;
	uint8_t obp0;
	uint8_t obp1;
	uint8_t wy;
	uint8_t wx;
	//Interrupt flags.
	union
	{
		struct
		{
			char v_blank    : 1;
			char lcd_status : 1;
			char timer      : 1;
			char serial     : 1;
			char joypad     : 1;
			char            : 3;
		}interrupt;
		char IF;
	};
	union
	{
		struct
		{
			char v_blank    : 1;
			char lcd_status : 1;
			char timer      : 1;
			char serial     : 1;
			char joypad     : 1;
			char            : 3;
		}enabled;
		char IE;
	};
	int IME;
	//Serial Registers;
	uint8_t serial_data;
	uint8_t serial_control;
}memory_t;
