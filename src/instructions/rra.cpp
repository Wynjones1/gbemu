#include "cpu.h"

void RRA(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_rra(state);
}

void cpu_rra(struct cpu_state *state)
{
	state->a = cpu_rr(state, state->a);
#if PROPER_FLAGS
    cpu_set_zero(state, 0);
#endif
}
