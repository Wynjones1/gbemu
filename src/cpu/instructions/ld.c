#include "cpu.h"

static REG_INPUT HL_REG = {.r16 = REG16_HL};

void LD(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_DATA16_UNSIGNED_INDIRECT)
	{
		if(arg1 == ARG_TYPE_REG16)
		{
			reg16_t data = cpu_load_reg16(state, i1);
			cpu_store16_indirect(state, i0, data);
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			reg_t data = cpu_load_reg8(state, i1);
			cpu_store8_indirect(state, i0, data);
		}
	}
	else if(arg0 == ARG_TYPE_HL_INDIRECT_DEC)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
			reg_t data = cpu_load_reg8(state, i1);
			cpu_store8_indirect(state, HL_REG, data);
			cpu_dec(state, HL_REG);
		}
	}
	else if(arg0 == ARG_TYPE_HL_INDIRECT_INC)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
			reg_t data = cpu_load_reg8(state, i1);
			cpu_store8_indirect(state, HL_REG, data);
			cpu_inc(state, HL_REG);
		}
	}
	else if(arg0 == ARG_TYPE_REG16)
	{
		reg16_t data;
		if(arg1 == ARG_TYPE_DATA16)
		{
			//TODO: Extract the firt arg from the stream.
		}
		else if(arg1 == ARG_TYPE_REG16)
		{
			data = cpu_load_reg16(state, i1);
		}
		else if(arg1 == ARG_TYPE_REL8_ADD_SP)
		{
			reg16_t addr = state->sp + cpu_load_reg8(state, i1);
			data = cpu_load16(state, addr);
		}
		cpu_store_reg16(state, i0, data);
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		reg_t data;
		if(arg1 == ARG_TYPE_DATA8)
		{
			//TODO: Extract the second arg from the stream.
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			data = cpu_load_reg8(state, i1);
		}
		cpu_store16_indirect(state, i0, data);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t data;
		if(arg1 == ARG_TYPE_DATA16_UNSIGNED_INDIRECT)
		{
			//TODO: Get the data at the address.
		}
		else if(arg1 == ARG_TYPE_DATA8)
		{
			//TODO: Get the data fromt the stream.
		}
		else if(arg1 == ARG_TYPE_HL_INDIRECT_DEC)
		{
			reg16_t addr = cpu_load_reg16(state, HL_REG);
			cpu_dec(state, HL_REG);
			data = cpu_load8(state, addr);
		}
		else if(arg1 == ARG_TYPE_HL_INDIRECT_INC)
		{
			reg16_t addr = cpu_load_reg16(state, HL_REG);
			cpu_inc(state, HL_REG);
			data = cpu_load8(state, addr);
		}
		else if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
			data = cpu_load16_indirect(state, i1);
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			data = cpu_load_reg8(state, i1);
		}
		else if(arg1 == ARG_TYPE_REG8_INDIRECT)
		{
			data = cpu_load8_indirect(state, i1);
		}
		cpu_store_reg8(state, i0, data);
	}
	else if(arg0 == ARG_TYPE_REG8_INDIRECT)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
			reg_t data = cpu_load_reg8(state, i1);
			cpu_store8_indirect(state, i0, data);
		}
	}
}
