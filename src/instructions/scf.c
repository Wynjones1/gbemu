#include "cpu.h"

void SCF(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	cpu_set_carry(state, 1);
    cpu_set_half_carry(state, 0);
    cpu_set_subtract(state, 0);
}
