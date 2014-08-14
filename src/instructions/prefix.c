#include "cpu.h"
#include "opcodes.h"

void PREFIX_CB( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	reg_t instruction = state->arg;
	struct opcode *op = &cb_op_table[instruction];
	state->pc += op->size - 1;
	op->op(state, op->arg0, op->i0, op->arg1, op->i1);
}
