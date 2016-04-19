#pragma once
#ifdef __cplusplus
extern "C" {
#endif

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
	uint32_t  tima_counter;
	uint32_t  div_counter;

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
void cpu_start(cpu_state_t *state);
void cpu_delete(cpu_state_t *state);
void cpu_save_state(cpu_state_t *state, const char *filename);
cpu_state_t *cpu_load_state(const char *filename);

reg_t   cpu_load_reg8(cpu_state_t *state, REG_INPUT reg);
reg16_t cpu_load_reg16(cpu_state_t *state, REG_INPUT reg);
void    cpu_store_reg8(cpu_state_t *state, REG_INPUT reg, reg_t data);
void    cpu_store_reg16(cpu_state_t *state, REG_INPUT reg, reg16_t data);
reg_t   cpu_load8(cpu_state_t *state, reg16_t addr);
reg16_t cpu_load16(cpu_state_t *state, reg16_t addr);
reg_t   cpu_load8_indirect(cpu_state_t *state, REG_INPUT reg);
reg16_t cpu_load16_indirect(cpu_state_t *state, REG_INPUT reg);
void    cpu_store8(cpu_state_t *state, reg16_t addr, reg_t data);
void    cpu_store16(cpu_state_t *state, reg16_t addr, reg16_t data);
void    cpu_store8_indirect(cpu_state_t *state, REG_INPUT reg, reg_t data);
void    cpu_store16_indirect(cpu_state_t *state, REG_INPUT reg, reg16_t data);
void    cpu_call(cpu_state_t *state, reg16_t addr);
reg_t   cpu_dec8(cpu_state_t *state, reg_t d0);
reg_t   cpu_inc8(cpu_state_t *state, reg_t d0);
void    cpu_dec16(cpu_state_t *state, REG_INPUT reg);
void    cpu_inc16(cpu_state_t *state, REG_INPUT reg);
reg_t   cpu_and(cpu_state_t *state, reg_t d0, reg_t d1);
reg_t   cpu_or(cpu_state_t *state, reg_t d0, reg_t d1);
reg_t   cpu_adc(cpu_state_t *state, reg_t d0, reg_t d1);
reg_t   cpu_add8(cpu_state_t *state, reg_t d0, reg_t d1);
reg16_t cpu_add16(cpu_state_t *state, reg16_t d0, reg16_t d1);
void    cpu_xor(cpu_state_t *state, reg_t d0);
void    cpu_sbc(cpu_state_t *state, reg_t d0);
void    cpu_sub(cpu_state_t *state, reg_t d0);
void    cpu_rst(cpu_state_t *state, reg_t d0);
void    cpu_push(cpu_state_t *state, reg16_t d0);
void    cpu_pop(cpu_state_t *state, REG_INPUT reg);
void    cpu_jump(cpu_state_t *state, reg16_t addr);
void    cpu_jump_rel(cpu_state_t *state, reg_t addr);
void    cpu_rra(cpu_state_t *state);
void    cpu_rlca(cpu_state_t *state);
void    cpu_rla(cpu_state_t *state);
void    cpu_rrca(cpu_state_t *state);
void    cpu_ret(cpu_state_t *state);
void    cpu_reti(cpu_state_t *state);
void    cpu_cmp(cpu_state_t *state, reg_t d0);
void    cpu_enable_interrupts(cpu_state_t *state);
void    cpu_disable_interrupts(cpu_state_t *state);
void    cpu_bit(cpu_state_t *state, reg_t pos, reg_t d0);
reg_t   cpu_res(cpu_state_t *state, reg_t pos, reg_t d0);
reg_t   cpu_set(cpu_state_t *state, reg_t pos, reg_t d0);
reg_t   cpu_rl(cpu_state_t *state, reg_t d0);
reg_t   cpu_rr(cpu_state_t *state, reg_t d0);
reg_t   cpu_rlc(cpu_state_t *state, reg_t d0);
reg_t   cpu_rrc(cpu_state_t *state, reg_t d0);
reg_t   cpu_sra(cpu_state_t *state, reg_t d0);
reg_t   cpu_sla(cpu_state_t *state, reg_t d0);
reg_t   cpu_srl(cpu_state_t *state, reg_t d0);
reg_t   cpu_swap(cpu_state_t *state, reg_t d0);
void    cpu_set_zero(cpu_state_t *state, reg_t d0);
void    cpu_set_carry(cpu_state_t *state, reg_t d0);
void    cpu_set_half_carry(cpu_state_t *state, reg_t d0);
void    cpu_set_subtract(cpu_state_t *state, reg_t d0);
int     cpu_half_carry(cpu_state_t *state);
int     cpu_subtract(cpu_state_t *state);
int     cpu_carry(cpu_state_t *state);
int     cpu_zero(cpu_state_t *state);


#ifdef __cplusplus
} // extern "C"
#endif