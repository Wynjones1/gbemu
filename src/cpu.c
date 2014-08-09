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

const char *reg_strings[] =
{
	"F",
	"A", 
	"C",
	"B", 
	"E",
	"D", 
	"L",
	"H"
};

const char *reg16_strings[] =
{
	"AF",
	"BC",
	"DE",
	"HL",
	"SP",
};

#define X(n) case ARG_TYPE_ ## n:sprintf(buf, #n);break
void print_arg(char *buf, struct cpu_state *state, struct opcode *op,enum ARG_TYPE arg, REG_INPUT r)
{
	uint8_t rel;
	switch(arg)
	{
		case ARG_TYPE_REG8:
			sprintf(buf, "%s", reg_strings[r.r8]);
			break;
		case ARG_TYPE_REG8_INDIRECT:
			sprintf(buf, "(%s)", reg_strings[r.r8]);
			break;
		case ARG_TYPE_REG16:
			sprintf(buf, "%s", reg16_strings[r.r16]);
			break;
		case ARG_TYPE_REG16_INDIRECT:
			sprintf(buf, "(%s)", reg16_strings[r.r16]);
			break;
		case ARG_TYPE_DATA8:
			sprintf(buf, "0x%02x", cpu_load8(state, state->pc + 1));
			break;
		case ARG_TYPE_DATA8_UNSIGNED:
			sprintf(buf, "0x%02x", cpu_load8(state, state->pc + 1));
			break;
		case ARG_TYPE_DATA8_UNSIGNED_INDIRECT:
			sprintf(buf, "(0x%02x)", cpu_load8(state, state->pc + 1));
			break;
		case ARG_TYPE_DATA16:
			sprintf(buf, "0x%02x", cpu_load16(state, state->pc + 1));
			break;
		case ARG_TYPE_DATA16_UNSIGNED:
			sprintf(buf, "0x%02x", cpu_load16(state, state->pc + 1));
			break;
		case ARG_TYPE_DATA16_UNSIGNED_INDIRECT:
			sprintf(buf, "(0x%02x)", cpu_load16(state, state->pc + 1));
			break;
		case ARG_TYPE_REL8:
			rel = cpu_load8(state, state->pc + 1);
			sprintf(buf, "pc+ %02d", *(int8_t*)&rel + 2);
			break;
		case ARG_TYPE_REL8_ADD_SP:
			rel = cpu_load8(state, state->pc + 1);
			sprintf(buf, "sp + %02d", *(int8_t*)&rel + 2);
			break;
		case ARG_TYPE_HL_INDIRECT_DEC:
			sprintf(buf, "(HL-)");
			break;
		case ARG_TYPE_HL_INDIRECT_INC:
			sprintf(buf, "(HL+)");
			break;
		X(00H);
		X(08H);
		X(10H);
		X(18H);
		X(20H);
		X(28H);
		X(30H);
		X(38H);
		X(1);
		X(2);
		X(3);
		X(4);
		X(5);
		X(6);
		X(7);
		X(Z);
		X(NC);
		X(NZ);
		case ARG_TYPE_NONE:
			buf[0] = 0;
			break;
	}
	
}
void print_op(char *buffer, struct cpu_state *state, struct opcode *op)
{
	char arg0[1024];
	char arg1[1024];
	print_arg(arg0, state, op,op->arg0, op->i0);
	print_arg(arg1, state, op,op->arg1, op->i1);
	sprintf(buffer, "0x%04x %s %s %s", state->pc, op->name, arg0, arg1);
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

#if 1
		char buf[1024];
		print_op(buf, state, op);
		Output("%s %d\n", buf, state->memory->boot_locked);
#endif
		op->op(state, op->arg0, op->i0, op->arg1, op->i1);
		//Increment program counter.
		if(!state->jump && instruction != 0xCB)
		{
			state->pc += op->size;
		}
		state->cycles += state->success ? op->success : op->fail;
		fflush(stdout);
	}
}

