#include "cpu.h"

void RETI(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_reti(state);
}

void cpu_reti(cpu_state_t *state)
{
    if(state->cont)
    {
        if(!--state->cont)
        {
            state->paused = 1;
        }
    }
	state->pc    = memory_load16(state->memory, state->sp);
	state->sp   += 2;
	cpu_enable_interrupts(state);
}
