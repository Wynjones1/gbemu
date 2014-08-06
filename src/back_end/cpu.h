#pragma once
#include "memory.h"
#include "display.h"
#include "events.h"
#include "types.h"
#include "common.h"
#include "arg_defs.h"

typedef struct cpu_state
{
	union
	{
		reg_t   registers[NUM_REGISTERS];
		reg16_t registers16[NUM_REGISTERS16];
		struct
		{
			/* represent the flag register as a reg_t and a bitfield */
			union
			{
				reg_t f;
				struct
				{
					reg_t             : 4;
					reg_t carry       : 1;
					reg_t half_carry  : 1;
					reg_t subtract    : 1;
					reg_t zero        : 1;
				};
			};
			/* the odd arraingment is to ensure correct accesses */
			reg_t a;
			reg_t c;
			reg_t b;
			reg_t e;
			reg_t d;
			reg_t l;
			reg_t h;
		};
	};
	reg16_t   sp;
	reg16_t   pc;
	memory_t  *memory;
	display_t *display;
	events_t  events;
	int       jump;
	int       success;
	int       cycles;
}cpu_state_t;

cpu_state_t *cpu_init(const char *boot_rom_filename);
void cpu_start(struct cpu_state *state);
void cpu_load_rom(struct cpu_state *state, const char *filename);
void cpu_delete(cpu_state_t *state);

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

void    cpu_dec8(struct cpu_state *state, REG_INPUT reg);
void    cpu_inc8(struct cpu_state *state, REG_INPUT reg);
void    cpu_dec16(struct cpu_state *state, REG_INPUT reg);
void    cpu_inc16(struct cpu_state *state, REG_INPUT reg);
reg_t   cpu_and(struct cpu_state *state, reg_t d0, reg_t d1);
reg_t   cpu_or(struct cpu_state *state, reg_t d0, reg_t d1);
reg_t   cpu_adc(struct cpu_state *state, reg_t d0, reg_t d1);
reg_t   cpu_add8(struct cpu_state *state, reg_t d0, reg_t d1);
reg_t   cpu_add16(struct cpu_state *state, reg16_t d0, reg16_t d1);
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
int     cpu_carry(struct cpu_state *state);
int     cpu_zero(struct cpu_state *state);
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
