#include "cpu.h"

void SLA(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		reg_t d0 = cpu_load8_indirect(state, i0);
		d0 = cpu_sla(state, d0);
		cpu_store8_indirect(state, i0, d0);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t d0 = cpu_load_reg8(state, i0);
		d0 = cpu_sla(state, d0);
		cpu_store_reg8(state, i0, d0);
	}
}
