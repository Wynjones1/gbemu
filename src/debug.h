#pragma once
#include "arg_defs.h"
#include "cpu.h"
#include "opcodes.h"

extern struct cpu_state *g_state;
void debug_output_framebuffer(struct cpu_state *state);
void debug_output_tile_maps(struct cpu_state *state);
void debug_output_registers(struct cpu_state *state);
void debug_print_arg(char *buf, struct cpu_state *state, const struct opcode *op, enum ARG_TYPE arg, REG_INPUT r);
void debug_print_op(char *buffer, struct cpu_state *state, const struct opcode *op);

void debug_on_exit(void);

void BREAK(void);
void BREAKIF(int cond);

void output_tiles(void);//TODO:Rename
