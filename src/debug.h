#pragma once
#include "arg_defs.h"
#include "cpu.h"
#include "opcodes.h"

extern cpu_state_t *g_state;
void debug_output_framebuffer(cpu_state_t *state);
void debug_output_tile_maps(cpu_state_t *state);
void debug_output_registers(cpu_state_t *state);
void debug_print_arg(char *buf, cpu_state_t *state,
                     struct opcode *op, enum ARG_TYPE arg, REG_INPUT r, bool print_values);
void debug_print_op(char *buffer, cpu_state_t *state, struct opcode *op);

void debug_init(void);

void BREAK(void);
void BREAKIF(int cond);

void output_tiles(void);//TODO:Rename

bool debug_is_break_address(int bank, uint16_t pc);
