#include <stdlib.h>
#include <stdio.h>

#include "cpu.h"
#include "opcodes.h"

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
	out->display     = display_init(out->memory);
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

void cpu_load_rom(struct cpu_state *state, const char *filename)
{
	memory_load_rom(state->memory, filename);
}

void cpu_start(struct cpu_state *state)
{
	state->pc = 0;
	while(1)
	{
		//Reset status flags.
		state->success = 1;
		state->jump    = 0;
		//Load instruction and execute it.
		reg_t instruction = cpu_load8(state, state->pc);
		struct opcode *op = &op_table[instruction];
		printf("0x%04x: \n", state->pc);
		op->op(state, op->arg0, op->i0, op->arg1, op->i1);
		//Increment program counter.
		if(!state->jump && instruction != 0xCB)
		{
			state->pc += op->size;
		}
		state->cycles += state->success ? op->success : op->fail;
	}
}

