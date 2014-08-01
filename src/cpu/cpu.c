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
	"ARG_TYPE_Z",
	"ARG_TYPE_NC",
	"ARG_TYPE_NZ",
	"ARG_TYPE_NONE"
};

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

/* CPU Helper functions */
reg_t   cpu_load_reg8(struct cpu_state *state, REG_INPUT reg)
{
	state->registers[reg.r8];
}

reg16_t cpu_load_reg16(struct cpu_state *state, REG_INPUT reg)
{
	state->registers16[reg.r16];
}

void cpu_store_reg8(struct cpu_state *state, REG_INPUT reg, reg_t data)
{
	
}

void cpu_store_reg16(struct cpu_state *state, REG_INPUT reg, reg16_t data)
{
	
}

/* Basic memory access functions */
reg_t cpu_load8(struct cpu_state *state, reg16_t addr)
{
	return state->memory[addr];
}
reg16_t cpu_load16(struct cpu_state *state, reg16_t addr)
{
	return *(reg16_t*)&state->memory[addr];
}

reg_t cpu_load8_indirect(struct cpu_state *state, REG_INPUT reg)
{
	reg16_t addr = state->registers16[reg.r16];
	return state->memory[addr];
}

reg16_t cpu_load16_indirect(struct cpu_state *state, REG_INPUT reg)
{
	reg16_t addr = state->registers16[reg.r16];
	return *(reg16_t*)&state->memory[addr];
}

void cpu_store8(struct cpu_state *state, reg16_t addr, reg_t data)
{
	state->memory[addr] = data;
}

void cpu_store16(struct cpu_state *state, reg16_t addr, reg16_t data)
{
	*(reg16_t*)&state->memory[addr] = data;
}

void cpu_store8_indirect(struct cpu_state *state, REG_INPUT reg, reg_t data)
{
	reg16_t addr = state->registers16[reg.r16];
	//TODO: Change to native add instruction...
	addr += 0xff00;
	state->memory[addr] = data;
}

void cpu_store16_indirect(struct cpu_state *state, REG_INPUT reg, reg16_t data)
{
	reg16_t addr = state->registers16[reg.r16];
	*(reg16_t*)&state->memory[addr] = data;
}

void cpu_dec(struct cpu_state *state, REG_INPUT reg)
{
	//TODO: Implement.
}

void cpu_inc(struct cpu_state *state, REG_INPUT reg)
{
	//TODO: Implement.
}
