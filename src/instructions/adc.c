#include "cpu.h"

void ADC(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REG8)
	{
		reg_t d0 = cpu_load_reg8(state, A_REG);
		reg_t d1;
		if(arg1 == ARG_TYPE_DATA8)
		{
			d1 = cpu_load8(state, state->pc + 1);
		}
		else if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
			d1 = cpu_load8_indirect(state, i1);
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			d1 = cpu_load_reg8(state, i1);
		}
		cpu_adc(state, d0, d1);
	}
}

reg_t cpu_adc(struct cpu_state *state, reg_t d0, reg_t d1)
{
	uint16_t res      = ((uint16_t)d0 + d1) + state->carry;
	state->zero       = (d0 & 0xff) == 0;
	state->subtract   = 0;
	state->half_carry = ((d0 & 0xf) + (d1 & 0xf) + state->carry) > 0xf;
	state->carry      = res > 0xff;
	return res & 0xff;
}