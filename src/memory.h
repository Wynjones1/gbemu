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
	reg_t stack[0x60];           // 0xff80 -> 0xfffe
	reg_t interrupt_enable;      // 0xffff

	//Boot Rom.
	int boot_locked;
	reg_t boot[0x100];
	//Banking information;
	int current_bank;
	int cart_type;
	//IO Status registers.
	int lcdc;
	int stat;
	int scy;
	int scx;
	int ly;
	int lyc;
	int dma;
	int bgp;
	int obp0;
	int obp1;
	int wy;
	int wx;
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
		};
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
}memory_t;
