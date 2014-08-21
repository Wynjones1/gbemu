#include "cpu.h"

void HALT(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	Error("0x%03x\n", state->pc);
	state->halt = 1;
}
