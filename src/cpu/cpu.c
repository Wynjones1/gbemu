#include <stdlib.h>
#include <stdio.h>

#include "cpu.h"
#include "opcodes.h"

const char *ARG_TYPE_s[] =
{
	"ARG_TYPE_REG8",
	"ARG_TYPE_REG8_INDIRECT",
	"ARG_TYPE_REG16",
	"ARG_TYPE_REG16_INDIRECT",
	"ARG_TYPE_DATA8",
	"ARG_TYPE_DATA8_UNSIGNED",
	"ARG_TYPE_DATA8_UNSIGNED_INDIRECT",
	"ARG_TYPE_DATA16",
	"ARG_TYPE_DATA16_UNSIGNED",
	"ARG_TYPE_DATA16_UNSIGNED_INDIRECT",
	"ARG_TYPE_REL8",
	"ARG_TYPE_REL8_ADD_SP",
	"ARG_TYPE_HL_INDIRECT_DEC",
	"ARG_TYPE_HL_INDIRECT_INC",
	"ARG_TYPE_00H",
	"ARG_TYPE_08H",
	"ARG_TYPE_10H",
	"ARG_TYPE_18H",
	"ARG_TYPE_20H",
	"ARG_TYPE_28H",
	"ARG_TYPE_30H",
	"ARG_TYPE_38H",
	"ARG_TYPE_1",
	"ARG_TYPE_2",
	"ARG_TYPE_3",
	"ARG_TYPE_4",
	"ARG_TYPE_5",
	"ARG_TYPE_6",
	"ARG_TYPE_7",
	"ARG_TYPE_Z",
	"ARG_TYPE_NC",
	"ARG_TYPE_NZ",
	"ARG_TYPE_NONE"
};

const char *cpu_get_arg_type_string(enum ARG_TYPE type)
{
	return ARG_TYPE_s[type];
}

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

cpu_state_t *cpu_init(const char *boot_rom_filename)
{
	cpu_state_t *out = calloc(1, sizeof(cpu_state_t));
	out->memory = memory_init(boot_rom_filename);
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
	state->registers[reg.r16] = data;
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
	state->rom = rom_read(filename);
}

void cpu_start(struct cpu_state *state)
{
	state->pc = 0;
	while(1)
	{
		reg_t instruction = cpu_load8(state, state->pc);
		struct opcode *op = &op_table[instruction];
		op->op(state, op->arg0, op->i0, op->arg1, op->i1);
		state->pc += op->size;
	}
}
