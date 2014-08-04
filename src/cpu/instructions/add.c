#include "cpu.h"

void ADD(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG16)
	{
		reg16_t d0 = cpu_load_reg16(state, i0);
		reg16_t d1;
		if(arg1 == ARG_TYPE_REG16)
		{
			d1 = cpu_load_reg16(state, i1);
		}
		else if(arg1 == ARG_TYPE_REL8)
		{
			d1 = state->memory[state->pc + 1];
		}
		reg16_t out = cpu_add16(state, d0, d1);
		cpu_store_reg16(state, i0, out);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t d0 = cpu_load_reg8(state, i0);
		reg_t d1;
		if(arg1 == ARG_TYPE_DATA8)
		{
			d1 = state->memory[state->pc + 1];
		}
		else if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
			d1 = cpu_load8_indirect(state, i1);
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			d1 = cpu_load_reg8(state, i1);
		}
		reg_t out = cpu_add8(state, d0, d1);
		cpu_store_reg8(state, i0, out);
	}
}
