#include "cpu.h"

void JR(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REL8
		|| (arg0 == ARG_TYPE_NC    && !cpu_carry(state))
		|| (arg0 == ARG_TYPE_NZ    && !cpu_zero(state))
		|| (arg0 == ARG_TYPE_Z     &&  cpu_zero(state))
		|| (arg0 == ARG_TYPE_REG8  &&  cpu_carry(state)))
	{
		reg_t rel = cpu_load8(state, state->pc + 1);
		cpu_jump_rel(state, rel);
	}
	else
	{
		state->success = 0;
	}
}

void  cpu_jump_rel(struct cpu_state *state, reg_t addr)
{
	// We need to perform addition on a unsigned 16 bit
	// value and a signed 8 bir value, we perform this
	// by expanding both to 32 bits, performing the
	// subtraction and casting back to 16 bits.
	addr += 2;
	int32_t t = (int32_t)*(int8_t*)&addr;
	int32_t pc = state->pc;
	state->jump = 1;
	state->pc = pc + t;
}
