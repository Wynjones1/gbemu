#include "cpu.h"

void SLA(cpu_state_t *state,
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

reg_t   cpu_sla(cpu_state_t *state, reg_t d0)
{
	cpu_set_carry(state, (d0 >> 7) & 0x1);
	d0 <<= 1;
	cpu_set_zero(state, (d0 == 0));
	cpu_set_half_carry(state, 0);
	cpu_set_subtract(state, 0);
	return d0;
}
