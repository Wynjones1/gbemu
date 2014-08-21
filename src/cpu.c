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
	memset(&out->events, 0x00, sizeof(events_t));
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
	}
	return 0;
}
#undef X

void handle_events(struct cpu_state *state)
{
	//Check for events.
	events_handle(&state->events);
	if(state->events.quit) exit(0);
}

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
		fprintf(fp, "%4.2f %fMhz\n", count / 1000.0, (clocks / count) / 1000.0);
	}
}

int temp[] = {0, 0x90, 0x91, 0x94};
void simulate_display(struct cpu_state *state)
{
#if !DISPLAY_THREAD
	static int time;
#endif
	if(state->clock_counter >= CPU_CLOCKS_PER_LINE) //This should take 16ms
	{
		state->clock_counter        -= CPU_CLOCKS_PER_LINE;
		state->memory->ly           += 1;
		state->memory->ly           %= 144 + 10;
		if(state->memory->ly == 144)
		{
			state->memory->interrupt.v_blank = 1;
		}
#if !DISPLAY_THREAD
		if(state->memory->ly == 144)
		{
			if(state->memory->lcdc.enabled)
			{
				display_display(state->display);
			}
		}
#endif
	}
}


void cpu_start(struct cpu_state *state)
{
	reg_t instruction;
	struct opcode *op;
	state->pc = 0;
#if !DISPLAY_THREAD
	display_display(state->display);
#endif
	SDL_Delay(1000);
	while(1)
	{
		//Reset status flags.
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
			int temp = state->success ? op->success : op->fail;
			state->clock_counter += temp;
			display_mhz(temp);
			simulate_display(state);
		}
	}
}

