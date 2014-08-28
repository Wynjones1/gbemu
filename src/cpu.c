#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>

#include "cpu.h"
#include "display.h"
#include "opcodes.h"
#include "ppm.h"
#include "debug.h"
#include "events.h"

#include <SDL2/SDL.h>

#if 0
#define CPU_ERROR(state, arg0, i0, arg1, i1)\
	fprintf(stderr, "%s %s %d %s %d\n", __func__, ARG_TYPE_s[arg0], i0.r8, ARG_TYPE_s[arg1], i1.r16);\
	exit(-1)
#else
	#define CPU_ERROR(state, arg0, i0, arg1, i1)
#endif

#define ERR()\
	fprintf(stderr, "%s %s %d %s %d %d\n", __func__, ARG_TYPE_s[arg0], i0.r8, ARG_TYPE_s[arg1], i1.r16, __LINE__);\
	exit(-1)

cpu_state_t *cpu_init(const char *boot_rom_filename, const char *rom)
{
	cpu_state_t *out = calloc(1, sizeof(cpu_state_t));
	out->memory      = memory_init(boot_rom_filename, rom);
	out->display     = display_init(out);
	out->frame_limit = 1;
	return out;
}

void cpu_delete(cpu_state_t *state)
{
	memory_delete(state->memory);
	free(state);
}

/* CPU Helper functions */
reg_t   cpu_load_reg8(struct cpu_state *state, REG_INPUT reg)
{
	return state->registers[reg.r8];
}

reg16_t cpu_load_reg16(struct cpu_state *state, REG_INPUT reg)
{
	return state->registers16[reg.r16];
}

void cpu_store_reg8(struct cpu_state *state, REG_INPUT reg, reg_t data)
{
	state->registers[reg.r8] = data;
}

void cpu_store_reg16(struct cpu_state *state, REG_INPUT reg, reg16_t data)
{
	state->registers16[reg.r16] = data;
}

int cpu_carry(struct cpu_state *state)
{
	return state->carry;
}

int cpu_zero(struct cpu_state *state)
{
	return state->zero;
}

#define X(n) state->memory->interrupt.n && state->memory->enabled.n
static int check_for_interrupts(struct cpu_state *state)
{
	//Check if interrupts are enabled
	reg16_t addr = 0;
	if(state->memory->IME && (state->memory->IF & state->memory->IE & 0x1f))
	{
		state->memory->IME = 0;
		state->halt        = 0;
		//Check each of the interrupts in priority order.
		if(X(v_blank))
		{
			state->memory->interrupt.v_blank = 0;
			addr = 0x0040;
		}
		else if(X(lcd_status))
		{
			state->memory->interrupt.lcd_status = 0;
			addr = 0x0048;
		}
		else if(X(timer))
		{
			state->memory->interrupt.timer = 0;
			addr = 0x0050;
		}
		else if(X(serial))
		{
			state->memory->interrupt.serial = 0;
			addr = 0x0058;
		}
		else if(X(joypad))
		{
			state->memory->interrupt.joypad = 0;
			addr = 0x0060;
		}
		Output("Interrupt 0x%04x\n", addr);
		cpu_push(state, state->pc);
		state->pc = addr;
		state->clock_counter += 5;
	}
	return 0;
}
#undef X

void display_mhz(int clk)
{
	static FILE *fp;
	static int count;
	static long unsigned int clocks;
	if(!fp)
	{
		fp = fopen("clock_speed.txt","w");
	}

	int temp = SDL_GetTicks();
	clocks += clk;
	if(temp - count > 1000)
	{
		count = temp;
		fprintf(fp, "%04.2f %04.2fMhz\n", count / 1000.0, (clocks / count) / 1000.0);
		fflush(fp);
	}
}

void frame_limit(int clk)
{
	#if 1
	static int scount;
	if(scount++ % 550 == 0)
	{
		SDL_Delay(1);
	}
	#endif
}

/*
* The div register increments 16384 times per second
* which is 256 clock cycles of time.
*/
void increment_div(cpu_state_t *state, int clk)
{
	static int count;
	count += clk;
	if(count >= 256)
	{
		state->memory->div++;
		count -= 256;
	}
}

#define X(n) if(count >= n){\
			count -= n;\
			state->memory->tima++;\
			if(state->memory->tima == 0){\
				state->memory->tima = state->memory->tma;\
				state->memory->interrupt.timer = 1;\
			}}
void increment_tima(cpu_state_t *state, int clk)
{
	static int count;
	if(state->memory->tac.enable)
	{
		count += clk;
		switch(state->memory->tac.clock_select)
		{
			case 0x0: //4.096Khz
				X(1024);
				break;
			case 0x1: //262.144Khz
				X(16);
				break;
			case 0x2: //65.536Khz
				X(64);
				break;
			case 0x3: //16.384Khz
				X(256);
				break;
		}
	}
}
#undef X
#if 0
	uint8_t scx = d->mem->scx;
	uint8_t scy = d->mem->scy;
	for(int j = 0; j < 8; j++)
	{
		for(int i = 0; i < 8; i++)
		{
			uint8_t shade = ((tile_data[1] >> i) & 0x1) << 1 |
							((tile_data[0] >> i) & 0x1);
			uint8_t x = tx * 8 + (7 - i);
			uint8_t y = ty * 8 + j;
			x = x - scx;
			y = y - scy;
			if(x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT)
			{
				write_pixel(data, pitch, x, y , GET_SHADE(d->mem->bgp, shade));
			}
		}
		tile_data += 2;
	}
#endif

void get_tile_data(cpu_state_t *state, int tx, int ty, int offset, uint8_t *data)
{
	//Tile map is located at address 0x9800 or 0x9c00
	uint8_t *video_ram = state->memory->video_ram;
	int tile_num = ty * 32 + tx;
	uint8_t  tile = video_ram[(state->memory->lcdc.map_select ? 0x1c00 : 0x1800) + tile_num];
	//Tils data is located at addresses
	// 0x8800 -> 97FF or
	// 0x8000 -> 8FFF
	uint8_t *tile_data;
	if(state->memory->lcdc.tile_select)
	{
		tile_data = &video_ram[tile * 16];
	}
	else
	{
		/* The tiles are in the range -128 to 127 so we cast the
		   binary representation of the tile to twos-complement */
		tile_data = &video_ram[0x1000 +  *(int8_t*)&tile * 16];
	}
	memcpy(data, tile_data + 2 * offset, 2);
}

uint8_t get_shade(uint8_t tile_data[2], int i)
{
	return ((tile_data[1] >> (7 - (i))) & 0x1) << 1 | ((tile_data[0] >> (7 - (i))) & 0x1);
}

void write_line(struct cpu_state *state)
{
	uint8_t tile_data[2];
	if(state->memory->ly < DISPLAY_HEIGHT)
	{
		int ty        = (state->memory->ly + state->memory->scy) / 8;
		int offset    = (state->memory->ly + state->memory->scy) % 8;
		int scx = state->memory->scx;
		uint8_t *data = g_video_data[state->memory->ly];
		for(int j = 0; j < 20; j++) //For each of the tiles in the line
		{
			for(int i = 0; i < 8; i++) //For each of the dots in the tile
			{
				int x  = j * 8 + i;
				int tx = j + (i + scx) / 8;
				int ox = (i + scx) % 8;
				uint8_t tile_data[2];
				get_tile_data(state, tx, ty, offset, tile_data);
				data[x] = get_shade(tile_data, ox);
			}
		}
	}
}

void simulate_display(struct cpu_state *state)
{
	if(state->clock_counter >= CPU_CLOCKS_PER_LINE) //This should take 16ms
	{
		write_line(state);
		state->clock_counter        -= CPU_CLOCKS_PER_LINE;
		state->memory->ly = (state->memory->ly + 1) % 154;
		if(state->memory->ly == state->memory->lyc)
		{
			state->memory->stat.coincidence = 1;
			if(state->memory->stat.coincidence_int)
			{
				state->memory->interrupt.lcd_status = 1;
			}
		}
		else
		{
			state->memory->stat.coincidence = 0;
		}

		if(state->memory->ly == 144)
		{
			state->memory->interrupt.v_blank = 1;
		}
	}
}


void cpu_start(struct cpu_state *state)
{
	g_state = state;
	atexit(debug_on_exit);
	reg_t instruction;
	struct opcode *op;
	state->pc = 0;
#if !DISPLAY_THREAD
	display_display(state->display);
#endif
	while(1)
	{
		//Reset status flags.
		state->step = 0;
		state->success = 1;
#if !DISPLAY_THREAD
		handle_events(state);
#endif
		check_for_interrupts(state);

		if(state->DI_Pending)
		{
			state->memory->IME = 0;
			state->DI_Pending = 0;
		}
		if(state->EI_Pending)
		{
			state->memory->IME = 1;
			state->EI_Pending = 0;
		}

		while(state->paused && !state->step && !state->slow)
		{
			SDL_Delay(100);
		}
		if(state->slow)
		{
			SDL_Delay(2);
		}

		//Load instruction and execute it.
		if(!state->halt)
		{
			instruction = cpu_load8(state, state->pc);
			op          = &op_table[instruction];
			if(op->size == 2 || instruction == 0xCB)
			{
				state->arg = cpu_load8(state,  state->pc + 1);
			}
			else if(op->size == 3)
			{
				state->arg = cpu_load16(state, state->pc + 1);
			}
			
	#if 0
			char buf[1024];
			debug_print_op(buf, state, op);
			Output("%s\n", buf);
	#endif


			state->pc  += op->size;
			op->op(state, op->arg0, op->i0, op->arg1, op->i1);
			//Increment program counter.
			int clk = state->success ? op->success : op->fail;
			state->clock_counter += clk;
			increment_div(state, clk);
			increment_tima(state, clk);
			display_mhz(clk);
			if(state->frame_limit)
			{
				frame_limit(clk);
			}
		}
		else
		{
			state->clock_counter += 4;
		}
		simulate_display(state);
	}
}

