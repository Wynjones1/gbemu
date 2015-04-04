#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>

#include "audio.h"
#include "cmdline.h"
#include "common.h"
#include "cpu.h"
#include "debug.h"
#include "display.h"
#include "events.h"
#include "logging.h"
#include "memory.h"
#include "opcodes.h"
#include "ppm.h"

cpu_state_t *cpu_init()
{
	cpu_state_t *out = (cpu_state_t*) calloc(1, sizeof(cpu_state_t));
	out->memory      = memory_init(out, cmdline_args.boot_rom, cmdline_args.in);
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
    return BIT_N(state->f, CARRY_BIT);
}

int cpu_zero(struct cpu_state *state)
{
    return BIT_N(state->f, ZERO_BIT);
}

int cpu_half_carry(struct cpu_state *state)
{
    return BIT_N(state->f, HALF_CARRY_BIT);
}

int cpu_subtract(struct cpu_state *state)
{
    return BIT_N(state->f, SUBTRACT_BIT);
}

#define X(field, value) (value) ? SET_N(state->f, field) : RESET_N(state->f, field)
void cpu_set_zero(struct cpu_state *state, reg_t d0)
{
    X(ZERO_BIT, d0);
}

void cpu_set_carry(struct cpu_state *state, reg_t d0)
{
    X(CARRY_BIT, d0);
}

void cpu_set_half_carry(struct cpu_state *state, reg_t d0)
{
    X(HALF_CARRY_BIT, d0);
}

void cpu_set_subtract(struct cpu_state *state, reg_t d0)
{
    X(SUBTRACT_BIT, d0);
}
#undef X

#define X(elem) fwrite(&state->elem, sizeof(state->elem), 1, fp)
void cpu_save_state(cpu_state_t *state, const char *filename)
{
	FILE *fp = common_fopen(filename, "wb");
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
	FILE *fp = FOPEN(filename, "rb");
	int temp, version;
	temp = fscanf(fp, "GBEMU%d", &version);
	if(temp != 1)
	{
        log_error("%s is not a savestate.\n");
	}
	if(version != VERSION)
	{
        log_warning("Timestamp of emulator does not match that of the save state.\n");
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

#define X(n, addr, addr_in)\
		if(BIT_N(state->memory->IF, n ## _BIT) && BIT_N(state->memory->IE,n ## _BIT)){                  \
			RESET_N(state->memory->IF, n ## _BIT);                                          \
			addr = addr_in;                                                          \
		}
static int check_for_interrupts(struct cpu_state *state)
{
	//Check if interrupts are enabled
	reg16_t addr = 0;
	if(state->memory->IME && (state->memory->IF & state->memory->IE & 0x1f))
	{
		state->memory->IME = 0;
		state->halt        = 0;
		//Check each of the interrupts in priority order.
			 X(VBLANK,     addr, 0x40)
		else X(LCD_STATUS, addr, 0x48)
		else X(TIMER,      addr, 0x50)
		else X(SERIAL,     addr, 0x58)
		else X(JOYPAD,     addr, 0x60)

		log_verbose("Interrupt 0x%04x\n", addr);
        if(state->cont)
        {
            state->cont += 1;
        }
		cpu_push(state, state->pc);
		state->pc = addr;
		state->clock_counter += 5;
	}
	return 0;
}
#undef X

static void frame_limit(int clk)
{
    uint32_t        target_percent= 100;
	uint32_t        sample_time   = 17; //Sample time in milliseconds
	uint32_t        sample_clocks = target_percent * sample_time * CPU_CLOCKS_PER_MS / 100;
    uint32_t        min_wait      = 2;
    static uint32_t clk_count     = 0;
    static uint32_t last_time     = 0;

	clk_count += clk;
	if(clk_count >= sample_clocks)
	{
        int delay;
#if SPINLOCK
        do
        {
            delay = (sample_time - (SDL_GetTicks()- last_time));
        }
        while(delay > 0);
#else
        delay = sample_time - (SDL_GetTicks()- last_time);
        if(delay > 0) SDL_Delay(delay);
#endif
        last_time  = SDL_GetTicks();
        clk_count -= sample_clocks;
	}
}

/*
* Sample the clock speed every second.
*/
static void record_clock_speed(int clk)
{
    static uint32_t time = 0;
    static uint64_t total;
    uint32_t wait_time = CPU_CLOCK_SPEED / 2;

    total += clk;
    if(total > wait_time)
    {
        g_state->fps = 100 * total  / (CPU_CLOCKS_PER_MS * ((SDL_GetTicks() - time)));
#if LOG_CLKSPEED
        log_message("CLKSPEED %07.02f%%", g_state->fps);
#endif
        total -= wait_time;
        time  = SDL_GetTicks();
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
				SET_N(state->memory->IF, TIMER_BIT);\
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

static void record(struct cpu_state *state)
{
    static FILE *fp;
    if(!fp) fp = FOPEN("record.txt", "wb");
    fwrite(&state->memory->buttons, 1, sizeof(state->memory->buttons), fp);
    fwrite(&state->memory->dpad,    1, sizeof(state->memory->dpad), fp);
}

static void replay(struct cpu_state *state)
{
    static FILE *fp;
    if(!fp) fp = FOPEN("record.txt", "rb");
    if(feof(fp)) exit(0);
    fread(&state->memory->buttons, 1, sizeof(state->memory->buttons), fp);
    fread(&state->memory->dpad,    1, sizeof(state->memory->dpad), fp);
}

static void log_instruction(struct opcode *op, reg16_t arg)
{
}

void cpu_start(struct cpu_state *state)
{
	g_state = state;
	atexit(debug_on_exit);
	reg_t instruction = 0;
	struct opcode *op = NULL;
	while(1)
	{
		if(state->store_state)
		{
			cpu_save_state(state, "game.state");
            return;
		}

        if(state->memory->boot_locked &&
            debug_is_break_address(state->memory->current_bank, state->pc))
        {
            state->paused = 1;
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
		check_for_interrupts(state);

		if(state->DI_Pending)
		{
			state->memory->IME = 0;
			state->DI_Pending  = 0;
		}
		if(state->EI_Pending)
		{
			state->memory->IME = 1;
			state->EI_Pending  = 0;
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

        log_instruction(op, state->arg);
#if 1
        static FILE *fp;
        if(!fp) fp = fopen("instr.txt", "w");
        if(!state->halt && state->memory->boot_locked)//state->paused)
        {
            char buf[100];
            debug_print_op(buf, state, op);
            fprintf(fp,"%s\n", buf);
        }
#endif
        
        if(cmdline_args.record)
            record(state);
        if(cmdline_args.replay)
            replay(state);

		op->op(state, op->arg0, op->i0, op->arg1, op->i1);

		//Increment program counter.
		int clk = state->success ? op->success : op->fail;
		state->clock_counter += clk;
		increment_div(state, clk);
		increment_tima(state, clk);
        audio_simulate(state->memory->audio, clk);
		if(state->frame_limit)
		{
			frame_limit(clk);
		}
        record_clock_speed(clk);
		display_simulate(state);
	}
}
