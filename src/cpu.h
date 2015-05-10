#pragma once
#include "memory.h"
#include "display.h"
#include "types.h"
#include "common.h"
#include "arg_defs.h"
#include "events.h"
#include "cmdline.h"

#define CPU_CLOCK_SPEED     (1 << 22) //4194304
#define CPU_CLOCKS_PER_MS   (CPU_CLOCK_SPEED / 1000.0f)
#define CPU_CLOCKS_PER_LINE (DISPLAY_MS_PER_LINE * CPU_CLOCKS_PER_MS)

#define CARRY_BIT      4
#define HALF_CARRY_BIT 5
#define SUBTRACT_BIT   6
#define ZERO_BIT       7

typedef struct cpu_state
{
	union
	{
		reg_t   registers[NUM_REGISTERS];
		reg16_t registers16[NUM_REGISTERS16];
		struct
		{
            reg_t f;
			reg_t a;
			reg_t c;
			reg_t b;
			reg_t e;
			reg_t d;
			reg_t l;
			reg_t h;
		};
		struct
		{
			reg16_t af;
			reg16_t bc;
			reg16_t de;
			reg16_t hl;
			reg16_t sp;
		};
	};

	reg16_t   pc;
	memory_t  *memory;
	display_t *display;
	int       success;
	int       DI_Pending;
	int       EI_Pending;
	reg16_t   arg;
	uint32_t  clock_counter;

    bool  halt;
	bool  paused;
	bool  step;
	bool  frame_limit;
	bool  slow;
	bool  store_state;
    bool  load_state;
    bool  exit;
    bool  cont;
    float fps;

    struct opcode *op;
}cpu_state_t;

cpu_state_t *cpu_init(void);
void cpu_start(struct cpu_state *state);
void cpu_delete(cpu_state_t *state);
void cpu_save_state(cpu_state_t *state, const char *filename);
cpu_state_t *cpu_load_state(const char *filename);

reg_t   cpu_load_reg8(struct cpu_state *state, REG_INPUT reg);
reg16_t cpu_load_reg16(struct cpu_state *state, REG_INPUT reg);
void cpu_store_reg8(struct cpu_state *state, REG_INPUT reg, reg_t data);
void cpu_store_reg16(struct cpu_state *state, REG_INPUT reg, reg16_t data);

reg_t   cpu_load8(struct cpu_state *state, reg16_t addr);
reg16_t cpu_load16(struct cpu_state *state, reg16_t addr);
reg_t   cpu_load8_indirect(struct cpu_state *state, REG_INPUT reg);
reg16_t cpu_load16_indirect(struct cpu_state *state, REG_INPUT reg);
void    cpu_store8(struct cpu_state *state, reg16_t addr, reg_t data);
void    cpu_store16(struct cpu_state *state, reg16_t addr, reg16_t data);
void    cpu_store8_indirect(struct cpu_state *state, REG_INPUT reg, reg_t data);
void    cpu_store16_indirect(struct cpu_state *state, REG_INPUT reg, reg16_t data);
void    cpu_call(cpu_state_t *state, reg16_t addr);
reg_t   cpu_dec8(struct cpu_state *state, reg_t d0);
reg_t   cpu_inc8(struct cpu_state *state, reg_t d0);
void    cpu_dec16(struct cpu_state *state, REG_INPUT reg);
void    cpu_inc16(struct cpu_state *state, REG_INPUT reg);
reg_t   cpu_and(struct cpu_state *state, reg_t d0, reg_t d1);
reg_t   cpu_or(struct cpu_state *state, reg_t d0, reg_t d1);
reg_t   cpu_adc(struct cpu_state *state, reg_t d0, reg_t d1);
reg_t   cpu_add8(struct cpu_state *state, reg_t d0, reg_t d1);
reg16_t cpu_add16(struct cpu_state *state, reg16_t d0, reg16_t d1);
void    cpu_xor(struct cpu_state *state, reg_t d0);
void    cpu_sbc(struct cpu_state *state, reg_t d0);
void    cpu_sub(struct cpu_state *state, reg_t d0);
void    cpu_rst(struct cpu_state *state, reg_t d0);
void    cpu_push(struct cpu_state *state, reg16_t d0);
void    cpu_pop(struct cpu_state *state, REG_INPUT reg);
void    cpu_jump(struct cpu_state *state, reg16_t addr);
void    cpu_jump_rel(struct cpu_state *state, reg_t addr);
void    cpu_rra(struct cpu_state *state);
void    cpu_rlca(struct cpu_state *state);
void    cpu_rla(struct cpu_state *state);
void    cpu_rrca(struct cpu_state *state);
void    cpu_ret(struct cpu_state *state);
void    cpu_reti(struct cpu_state *state);
void    cpu_cmp(struct cpu_state *state, reg_t d0);
void    cpu_enable_interrupts(struct cpu_state *state);
void    cpu_disable_interrupts(struct cpu_state *state);
void    cpu_bit(struct cpu_state *state, reg_t pos, reg_t d0);
reg_t   cpu_res(struct cpu_state *state, reg_t pos, reg_t d0);
reg_t   cpu_set(struct cpu_state *state, reg_t pos, reg_t d0);
reg_t   cpu_rl(struct cpu_state *state, reg_t d0);
reg_t   cpu_rr(struct cpu_state *state, reg_t d0);
reg_t   cpu_rlc(struct cpu_state *state, reg_t d0);
reg_t   cpu_rrc(struct cpu_state *state, reg_t d0);
reg_t   cpu_sra(struct cpu_state *state, reg_t d0);
reg_t   cpu_sla(struct cpu_state *state, reg_t d0);
reg_t   cpu_srl(struct cpu_state *state, reg_t d0);
reg_t   cpu_swap(struct cpu_state *state, reg_t d0);
void    cpu_set_zero(struct cpu_state *state, reg_t d0);
void    cpu_set_carry(struct cpu_state *state, reg_t d0);
void    cpu_set_half_carry(struct cpu_state *state, reg_t d0);
void    cpu_set_subtract(struct cpu_state *state, reg_t d0);
int     cpu_half_carry(struct cpu_state *state);
int     cpu_subtract(struct cpu_state *state);
int     cpu_carry(struct cpu_state *state);
int     cpu_zero(struct cpu_state *state);
