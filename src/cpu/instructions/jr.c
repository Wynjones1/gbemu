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
}
