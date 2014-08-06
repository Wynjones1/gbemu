#include "cpu.h"

void RET(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_NONE
		|| (arg0 == ARG_TYPE_NC   && !cpu_carry(state))
		|| (arg0 == ARG_TYPE_NZ   && !cpu_zero(state))
		|| (arg0 == ARG_TYPE_REG8 &&  cpu_carry(state)) //Actually the carry flag.
		|| (arg0 == ARG_TYPE_Z    &&  cpu_zero(state)))
	{
		cpu_ret(state);
	}
	else
	{
		state->success = 0;
	}
}

void cpu_ret(struct cpu_state *state)
{
	state->jump  = 1;
	state->pc    = memory_load16(state->memory, state->sp);
	state->sp   += 2;
}
