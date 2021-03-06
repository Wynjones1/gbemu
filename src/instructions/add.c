#include "cpu.h"

void ADD(cpu_state_t *state,
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
            reg16_t out = cpu_add16(state, d0, d1);
            cpu_store_reg16(state, i0, out);
		}
		else if(arg1 == ARG_TYPE_REL8)//Always SP
		{
            uint8_t d8 = (uint8_t)state->arg;
            cpu_set_zero(state, 0);
            cpu_set_subtract(state, 0);
            cpu_set_half_carry(state, (state->sp & 0x0f) + (d8 & 0x0f) > 0x0f);
            cpu_set_carry(state, (state->sp & 0xff) + (d8 & 0xff) > 0xff);
            state->sp = state->sp + *(int8_t*)&d8;
		}
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t d0 = cpu_load_reg8(state, i0);
		reg_t d1;
		if(arg1 == ARG_TYPE_DATA8)
		{
			d1 = (reg_t)state->arg;
		}
		else if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
			d1 = cpu_load8_indirect(state, i1);
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			d1 = cpu_load_reg8(state, i1);
		}
		else
		{
			Error("Invalid argument type.\n");
		}
		reg_t out = cpu_add8(state, d0, d1);
		cpu_store_reg8(state, i0, out);
	}
}

reg_t cpu_add8(cpu_state_t *state, reg_t d0, reg_t d1)
{
	uint16_t t0       = (uint16_t)d0 + (uint16_t)d1;
	cpu_set_zero(state, (t0 & 0xff) == 0);
	cpu_set_subtract(state, 0);
	cpu_set_half_carry(state, (d0 & 0xf) + (d1 & 0xf) > 0xf);
	cpu_set_carry(state, t0 > 0xff);
	return t0 & 0xff;
}

reg16_t cpu_add16(cpu_state_t *state, reg16_t d0, reg16_t d1)
{
	uint32_t t0       = (uint32_t)d0 + (uint32_t)d1;
	cpu_set_subtract(state, 0);
	cpu_set_carry(state, t0 > 0xffff);
	cpu_set_half_carry(state, (d0 & 0xfff) + (d1 & 0xfff) > 0xfff);
	return t0 & 0xffff;
}
