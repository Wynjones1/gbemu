#include "cpu.h"

static REG_INPUT HL_REG = {.r16 = REG16_HL};

void LD(struct cpu_state *state,
		const const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_DATA16_UNSIGNED_INDIRECT)
	{
		if(arg1 == ARG_TYPE_REG16) //Always SP
		{
			cpu_store16_indirect(state, i0, state->sp);
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			reg_t   data = cpu_load_reg8(state, i1);
			reg16_t addr = state->arg;
			cpu_store8(state, addr, data);
		}
	}
	else if(arg0 == ARG_TYPE_HL_INDIRECT_DEC)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
			reg_t data = cpu_load_reg8(state, i1);
			cpu_store8_indirect(state, HL_REG, data);
			state->registers16[REG16_HL] -= 1;
		}
	}
	else if(arg0 == ARG_TYPE_HL_INDIRECT_INC)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
			reg_t data = cpu_load_reg8(state, i1);
			cpu_store8_indirect(state, HL_REG, data);
			state->registers16[REG16_HL] += 1;
		}
	}
	else if(arg0 == ARG_TYPE_REG16)
	{
		reg16_t data;
		if(arg1 == ARG_TYPE_DATA16)
		{
			data = state->arg;
		}
		else if(arg1 == ARG_TYPE_REG16)
		{
			data = cpu_load_reg16(state, i1);
		}
		else if(arg1 == ARG_TYPE_REL8_ADD_SP)
		{
			//TODO:Check that the signed addition is correct.
			reg_t d = state->arg;
			reg16_t addr    = cpu_add8(state, state->sp, d);
			state->zero     = 0;
			state->subtract = 0;
			data = cpu_load16(state, addr);
		}
		cpu_store_reg16(state, i0, data);
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		reg_t data;
		if(arg1 == ARG_TYPE_DATA8)
		{
			data = state->arg;
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			data = cpu_load_reg8(state, i1);
		}
		cpu_store8_indirect(state, i0, data);
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		reg_t data;
		if(arg1 == ARG_TYPE_DATA16_UNSIGNED_INDIRECT)
		{
			reg16_t addr = state->arg;
			data = cpu_load8(state, addr);
		}
		else if(arg1 == ARG_TYPE_DATA8)
		{
			data = state->arg;
		}
		else if(arg1 == ARG_TYPE_HL_INDIRECT_DEC)
		{
			reg16_t addr = cpu_load_reg16(state, HL_REG);
			state->registers16[REG16_HL] -= 1;
			data = cpu_load8(state, addr);
		}
		else if(arg1 == ARG_TYPE_HL_INDIRECT_INC)
		{
			reg16_t addr = cpu_load_reg16(state, HL_REG);
			state->registers16[REG16_HL] += 1;
			data = cpu_load8(state, addr);
		}
		else if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
			data = cpu_load8_indirect(state, i1);
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
			data = cpu_load_reg8(state, i1);
		}
		else if(arg1 == ARG_TYPE_REG8_INDIRECT) //Always the C register.
		{
			reg16_t addr = state->c + 0xff00;
			data = cpu_load8(state, addr);
		}
		cpu_store_reg8(state, i0, data);
	}
	else if(arg0 == ARG_TYPE_REG8_INDIRECT) //Always the C register.
	{
		if(arg1 == ARG_TYPE_REG8)
		{
			reg_t data = cpu_load_reg8(state, i1);
			reg16_t addr = state->c + 0xff00;
			cpu_store8(state, addr, data);
		}
	}
}

/* Basic memory access functions */
reg_t cpu_load8(struct cpu_state *state, reg16_t addr)
{
	return memory_load8(state->memory, addr);
}
reg16_t cpu_load16(struct cpu_state *state, reg16_t addr)
{
	return memory_load16(state->memory, addr);
}

reg_t cpu_load8_indirect(struct cpu_state *state, REG_INPUT reg)
{
	reg16_t addr = state->registers16[reg.r16];
	return memory_load8(state->memory, addr);
}

reg16_t cpu_load16_indirect(struct cpu_state *state, REG_INPUT reg)
{
	reg16_t addr = state->registers16[reg.r16];
	return memory_load16(state->memory, addr);
}

void cpu_store8(struct cpu_state *state, reg16_t addr, reg_t data)
{
	memory_store8(state->memory, addr, data);
}

void cpu_store16(struct cpu_state *state, reg16_t addr, reg16_t data)
{
	memory_store16(state->memory, addr, data);
}

void cpu_store8_indirect(struct cpu_state *state, REG_INPUT reg, reg_t data)
{
	reg16_t addr = state->registers16[reg.r16];
	memory_store8(state->memory, addr, data);
}

void cpu_store16_indirect(struct cpu_state *state, REG_INPUT reg, reg16_t data)
{
	reg16_t addr = state->registers16[reg.r16];
	memory_store16(state->memory, addr, data);
}
