#include "cpu.h"

void JR(cpu_state_t *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_REL8
		|| (arg0 == ARG_TYPE_NC    && !cpu_carry(state))
		|| (arg0 == ARG_TYPE_NZ    && !cpu_zero(state))
		|| (arg0 == ARG_TYPE_Z     &&  cpu_zero(state))
		|| (arg0 == ARG_TYPE_REG8  &&  cpu_carry(state)))
	{
		cpu_jump_rel(state, (reg_t)state->arg);
	}
	else
	{
		state->success = 0;
	}
}

void  cpu_jump_rel(cpu_state_t *state, reg_t addr)
{
	// We need to perform addition on a unsigned 16 bit
	// value and a signed 8 bir value, we perform this
	// by expanding both to 32 bits, performing the
	// subtraction and casting back to 16 bits.
	//TODO:Clean up to and make sure it is correct.
	int16_t t = *(int8_t*)&addr;
	int16_t pc = state->pc;
	state->pc = pc + t;
}
