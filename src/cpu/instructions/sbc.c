#include "cpu.h"

void SBC(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	reg_t data;
	if(arg1 == ARG_TYPE_DATA8)
	{
		data = cpu_load8(state, state->pc + 1);
	}
	else if(arg1 == ARG_TYPE_REG16_INDIRECT)
	{
		data = cpu_load8_indirect(state, i1);
	}
	else if(arg1 == ARG_TYPE_REG8)
	{
		data = cpu_load_reg8(state, i1);
	}
	cpu_sbc(state, data);
}
