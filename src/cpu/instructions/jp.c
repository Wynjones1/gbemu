#include "cpu.h"

void JP(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	reg16_t addr = *(reg16_t*)&state->memory[state->pc + 1];
	if(arg0 == ARG_TYPE_DATA16_UNSIGNED
		|| (arg0 == ARG_TYPE_NC    && !cpu_carry(state))
		|| (arg0 == ARG_TYPE_NZ    && !cpu_zero(state))
		|| (arg0 == ARG_TYPE_Z     &&  cpu_zero(state))
		|| (arg0 == ARG_TYPE_REG8  &&  cpu_carry(state)))
	{
		cpu_jump(state, addr);
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		addr = cpu_load16_indirect(state, i0);
		cpu_jump(state, addr);
	}
}
