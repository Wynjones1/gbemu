#include "cpu.h"

void POP(cpu_state_t *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	cpu_pop(state, i0); 
}

void cpu_pop(cpu_state_t *state, REG_INPUT reg)
{
	state->registers16[reg.r16]  = cpu_load16(state, state->sp);
	state->sp += 2;
    //If AF is specified the bottom 4bits of f cannot be set.
    //We just mask for all cases.
    state->f &= 0xf0;
}
