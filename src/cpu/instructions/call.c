#include "cpu.h"

/*
* Push the address of the next instruction to the stack
* and jump to the address in the instruction.
*/

void CALL(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_DATA16_UNSIGNED
		|| (arg0 == ARG_TYPE_NC   && !cpu_carry(state))
		|| (arg0 == ARG_TYPE_NZ   && !cpu_zero(state))
		|| (arg0 == ARG_TYPE_REG8 &&  cpu_carry(state)) //Actually the carry flag.
		|| (arg0 == ARG_TYPE_Z    &&  cpu_zero(state)))
	{
		reg16_t next_instruction = state->pc + 3;
		reg16_t jump = *(reg16_t*)&state->memory[state->pc + 1];
		cpu_push(state, next_instruction);
		cpu_jump(state, jump);
	}
}
