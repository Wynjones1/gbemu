#pragma once
#include "types.h"
#include "audio.h"
#include "rom.h"
#include <stdio.h>

typedef struct memory memory_t;
typedef struct cpu    cpu_t;

memory_t      *memory_init(cpu_state_t *state, const char *boot, const char *rom);
void           memory_delete(memory_t *mem);
reg_t          memory_load8(memory_t *mem, reg16_t addr);
void           memory_store8(memory_t *mem, reg16_t addr, reg_t data);
reg16_t        memory_load16(memory_t *mem, reg16_t addr);
void           memory_store16(memory_t *mem, reg16_t addr, reg16_t data);
const uint8_t *memory_get_tile_data(memory_t *memory, int tx, int ty, int offset, int map);
uint8_t        memory_get_tile_index(memory_t *state, int tx, int ty, int map);
void           memory_save_state(memory_t *memory, FILE *fp);
memory_t      *memory_load_state(FILE *fp);
audio_t       *memory_get_audio(memory_t *memory);

#define PALETTE_GBC_BIT 0 //GBC Only (placeholder)
#define BANK_BIT        3 //GBC Only (placeholder)
#define PALETTE_BIT     4
#define X_FLIP_BIT      5
#define Y_FLIP_BIT      6
#define PRIORITY_BIT    7

#define VBLANK_BIT     0
#define LCD_STATUS_BIT 1
#define TIMER_BIT      2
#define SERIAL_BIT     3
#define JOYPAD_BIT     4

struct OAM_data
{
	uint8_t y_pos;
	uint8_t x_pos;
	uint8_t tile;
    uint8_t flags;
};

typedef struct memory
{
	reg_t *bank_0;               // 0x0000 -> 0x3fff
	reg_t *bank_n;               // 0x4000 -> 0x7fff
	reg_t  video_ram[0x2000];    // 0x8000 -> 0x9fff
	reg_t *external_ram;         // 0xa000 -> 0xbfff
	reg_t working_ram_0[0x1000]; // 0xc000 -> 0xcfff
	reg_t working_ram_1[0x1000]; // 0xd000 -> 0xdfff //CGB: Should be switchable.
	reg_t *echo       ;          // 0xe000 -> 0xfdff
	union // 0xfe00 -> 0xfe9f
	{
		reg_t OAM[0xa0];
		struct OAM_data oam_data[40];
	};
    uint8_t oam_index_sort[40];
	//unused                     // 0xfea0 -> 0xfeff
	reg_t io_registers[0x80];    // 0xff00 -> 0xff7f
	reg_t stack[0x7F];           // 0xff80 -> 0xfffe
	//reg_t interrupt_enable;    // 0xffff //Further down.
	reg_t backup[0x10000];

	//Boot Rom.
	int boot_locked;
	reg_t boot[0x100];
	//Banking information and game rom data.
    uint8_t current_bank;
    uint8_t ram_bank;
	enum CART_TYPE cart_type;
	enum SAVE_RAM_SIZE  ram_size;
	enum ROM_SIZE       rom_size;
	int ram_enabled;
	int rom_ram_mode;
	//IO Status registers.
    struct
    {
        uint8_t bg_display       : 1;
        uint8_t obj_enable       : 1;
        uint8_t obj_size         : 1;
        uint8_t map_select       : 1;
        uint8_t tile_data_select : 1;
        uint8_t window_display   : 1;
        uint8_t window_map       : 1;
        uint8_t enabled          : 1;
    }lcdc;
	struct
	{
		uint8_t a        : 1;
		uint8_t b        : 1;
		uint8_t select   : 1;
		uint8_t start    : 1;
		uint8_t          : 1;
		uint8_t enabled  : 1;
		uint8_t          : 2;
	}buttons;
	struct
	{
		uint8_t right     : 1;
		uint8_t left      : 1;
		uint8_t up        : 1;
		uint8_t down      : 1;
		uint8_t enabled   : 1;
		uint8_t           : 3;
	}dpad;
	//Video Registers.
	struct
	{
		uint8_t mode            : 2;
		uint8_t coincidence     : 1;
		uint8_t h_blank_int     : 1;
		uint8_t v_blank_int     : 1;
		uint8_t oam_int         : 1;
		uint8_t coincidence_int : 1;
		uint8_t                 : 1;
	}stat;
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
    uint8_t IF;
    uint8_t IE;
	int IME;
	//Serial Registers
	uint8_t serial_data;
	uint8_t serial_control;
	//Timer Registers
	uint8_t div;
	uint8_t tima;
	uint8_t tma;
	struct
	{
		uint8_t clock_select : 2;
		uint8_t enable       : 1;
		uint8_t              : 5;
	}tac;

	cpu_state_t *state;
	audio_t     *audio;

	size_t to_read;
}memory_t;
