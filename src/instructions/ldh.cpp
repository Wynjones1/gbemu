#include "cpu.h"

void LDH(struct cpu_state *state,
		ARG_TYPE arg0, REG_INPUT i0,
		ARG_TYPE arg1, REG_INPUT i1)
{
	reg16_t addr = state->arg;
	addr += 0xff00;
	if(arg0 == ARG_TYPE_DATA8_UNSIGNED_INDIRECT)
	{
		reg_t data = cpu_load_reg8(state, i1);
		cpu_store8(state, addr, data);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t data = cpu_load8(state, addr);
		cpu_store_reg8(state, i0, data);
	}
}
