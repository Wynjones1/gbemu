#pragma once
#include "arg_defs.h"
#include "cpu.h"
#include "opcodes.h"

void debug_output_tile_maps(struct cpu_state *state);
void debug_output_registers(struct cpu_state *state);
void debug_print_arg(char *buf, struct cpu_state *state, struct opcode *op, enum ARG_TYPE arg, REG_INPUT r);
void debug_print_op(char *buffer, struct cpu_state *state, struct opcode *op);