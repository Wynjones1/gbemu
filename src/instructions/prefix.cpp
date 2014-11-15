#include "cpu.h"
#include "opcodes.h"

void PREFIX_CB( struct cpu_state *state, ARG_TYPE arg0, REG_INPUT i0, ARG_TYPE arg1, REG_INPUT i1)
{
	reg_t instruction = state->arg;
	const struct opcode *op = &cb_op_table[instruction];
	state->pc += op->size - 1;
	op->op(state, op->arg0, op->i0, op->arg1, op->i1);
}
