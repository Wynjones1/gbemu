#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "cpu.h"
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
	#if 0
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
			state->memory->tma++;\
			if(state->memory->tima == 0){\
				state->memory->tima = state->memory->tma;\
			}}
void increment_tima(cpu_state_t *state, int clk)
{
	static int count;
	if(!state->memory->tac.stop)
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

void simulate_display(struct cpu_state *state)
{
	if(state->clock_counter >= CPU_CLOCKS_PER_LINE) //This should take 16ms
	{
		state->clock_counter        -= CPU_CLOCKS_PER_LINE;
		state->memory->ly           += 1;
		state->memory->ly           %= 144 + 10;
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
		while(state->paused) SDL_Delay(100);
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
	#if 0
			debug_output_registers(state);
	#endif
			op->op(state, op->arg0, op->i0, op->arg1, op->i1);
			//Increment program counter.
			int clk = state->success ? op->success : op->fail;
			state->clock_counter += clk;
			increment_div(state, clk);
			increment_tima(state, clk);
			display_mhz(clk);
			simulate_display(state);
		}
	}
}

