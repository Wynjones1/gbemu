#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>

#include "common.h"
#include "cpu.h"
#include "display.h"
#include "opcodes.h"
#include "ppm.h"
#include "debug.h"
#include "events.h"
#include "memory.h"

#include <SDL2/SDL.h>

cpu_state_t *cpu_init(const char *boot_rom_filename, const char *rom)
{
	cpu_state_t *out = (cpu_state_t*)calloc(1, sizeof(cpu_state_t));
	out->memory      = memory_init(out, boot_rom_filename, rom);
	out->display     = display_init(out);
	out->frame_limit = 1;
	out->pc = 0;
	out->store_state = 0;
	return out;
}

void cpu_delete(cpu_state_t *state)
{
	memory_delete(state->memory);
	display_delete(state->display);
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

#define X(n, addr_in) if(state->memory->interrupt.n && state->memory->enabled.n)\
					{ state->memory->interrupt.n = 0; addr = addr_in;}
static int check_for_interrupts(struct cpu_state *state)
{
	//Check if interrupts are enabled
	reg16_t addr = 0;
	if(state->memory->IME && (state->memory->IF & state->memory->IE & 0x1f))
	{
		state->memory->IME = 0;
		state->halt        = 0;
		//Check each of the interrupts in priority order.
			 X(v_blank   , 0x40)
		else X(lcd_status, 0x48)
		else X(timer     , 0x50)
		else X(serial    , 0x58)
		else X(joypad    , 0x60)

		Output("Interrupt 0x%04x\n", addr);
		cpu_push(state, state->pc);
		state->pc = addr;
		state->clock_counter += 5;
	}
	return 0;
}
#undef X

static void display_mhz(int clk)
{
	static FILE *fp;
	if(!fp)
	{
		fp = FOPEN("clock_speed.txt","w");
	}

	static long unsigned int count, time_count;
	count += clk;
	const float c = 1.0;
	int temp = SDL_GetTicks();
	if(temp - time_count > c * 1000)
	{
		float s = (c * count) / CPU_CLOCK_SPEED;
		fprintf(fp, "%4.2f %f.\n", temp / 1000.0, s);
		time_count = temp;
		count = 0;
	}
}

static void frame_limit(int clk)
{
	const float sample_time = 0.01; //Sample time in seconds
	static long int clk_count, time_count;
	const int sample_clocks= sample_time * CPU_CLOCK_SPEED;

	clk_count += clk;
	if(clk_count > sample_clocks)
	{
		int temp = SDL_GetTicks();
		clk_count -= sample_clocks;
		int sleep_time = sample_time * 1000 - (temp - time_count);
		if(sleep_time > 0)
		{
			SDL_Delay(sleep_time);
		}
		time_count = temp;
	}
}

/*
* The div register increments 16384 times per second
* which is 256 clock cycles of time.
*/
static void increment_div(cpu_state_t *state, int clk)
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
static void increment_tima(cpu_state_t *state, int clk)
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

void cpu_start(struct cpu_state *state)
{
	g_state = state;
	atexit(debug_on_exit);
	reg_t instruction;
	struct opcode *op;
	//state->pc = 0;
#if !DISPLAY_THREAD
	display_display(state->display);
#endif
	while(1)
	{
		if(state->store_state)
		{
			cpu_save_state(state, "game.state");
			exit(0);
		}
		//Halt emulation.
		while(state->paused && !state->step && !state->slow)
		{
			SDL_Delay(100);
		}
		if(state->slow)
		{
			SDL_Delay(2);
		}

		//Reset branch success flags.
		state->step    = 0;
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


		//Load instruction and execute it.
		if(!state->halt)
		{
			instruction = cpu_load8(state, state->pc);
			op          = &op_table[instruction];
			//If present load the argument of the op.
			if(op->size == 2 || instruction == 0xCB)
			{
				state->arg = cpu_load8(state,  state->pc + 1);
			}
			else if(op->size == 3)
			{
				state->arg = cpu_load16(state, state->pc + 1);
			}
			state->pc  += op->size;
		}
		else
		{
			op = &op_table[0]; //NOP
		}
			
	#if 0
		char buf[1024];
		debug_print_op(buf, state, op);
		Output("%s\n", buf);
	#endif


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
		display_simulate(state);
	}
}

#define X(elem) fwrite(&state->elem, sizeof(state->elem), 1, fp)
void cpu_save_state(cpu_state_t *state, const char *filename)
{
	FILE *fp = FOPEN(filename, "w");
	fprintf(fp, "GBEMU%d ", VERSION);
	fwrite(state->registers, sizeof(reg_t), NUM_REGISTERS, fp);
	X(pc);
	X(success);
	X(DI_Pending);
	X(EI_Pending);
	X(arg);
	X(clock_counter);
	X(halt);
	X(paused);
	X(step);
	X(frame_limit);
	X(slow);
	memory_save_state(state->memory, fp);

	fclose(fp);
}
#undef X

#define X(elem) temp = fread(&state->elem, sizeof(state->elem), 1, fp)
cpu_state_t *cpu_load_state(const char *filename)
{
	cpu_state_t *state = (cpu_state_t*) malloc(sizeof(cpu_state_t));
	FILE *fp = FOPEN(filename, "r");
	int temp, version;
	temp = fscanf(fp, "GBEMU%d", &version);
	if(temp != 1)
	{
		Error("%s is not a savestate.\n");
	}
	if(version != VERSION)
	{
		Warning("Timestamp of emulator does not match that of the save state.\n");
	}
	temp = getc(fp);
	temp = fread(state->registers, sizeof(reg_t), NUM_REGISTERS, fp);
	X(pc);
	X(success);
	X(DI_Pending);
	X(EI_Pending);
	X(arg);
	X(clock_counter);
	X(halt);
	X(paused);
	X(step);
	X(frame_limit);
	X(slow);
	state->memory = memory_load_state(fp);
	state->display = display_init(state);
	fclose(fp);
	return state;
}
#undef X
