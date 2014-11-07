#include "cpu.h"

/*
* Push the address of the next instruction to the stack
* and jump to the address in the instruction.
*/

void CALL(struct cpu_state *state,
		ARG_TYPE arg0, REG_INPUT i0,
		ARG_TYPE arg1, REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_DATA16_UNSIGNED
		|| (arg0 == ARG_TYPE_NC   && !cpu_carry(state))
		|| (arg0 == ARG_TYPE_NZ   && !cpu_zero(state))
		|| (arg0 == ARG_TYPE_REG8 &&  cpu_carry(state)) //Actually the carry flag.
		|| (arg0 == ARG_TYPE_Z    &&  cpu_zero(state)))
	{
		cpu_call(state, state->arg);
	}
	else
	{
		state->success = 0;
	}
}

void cpu_call(cpu_state_t *state, reg16_t addr)
{
	reg16_t jump = addr;
	cpu_push(state, state->pc);
	cpu_jump(state, jump);
}
