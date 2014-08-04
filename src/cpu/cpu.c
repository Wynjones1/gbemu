#include <stdlib.h>
#include <stdio.h>

#include "cpu.h"
#include "opcodes.h"

const char *ARG_TYPE_s[] =
{
	"ARG_TYPE_REG8",
	"ARG_TYPE_REG8_INDIRECT",
	"ARG_TYPE_REG16",
	"ARG_TYPE_REG16_INDIRECT",
	"ARG_TYPE_DATA8",
	"ARG_TYPE_DATA8_UNSIGNED",
	"ARG_TYPE_DATA8_UNSIGNED_INDIRECT",
	"ARG_TYPE_DATA16",
	"ARG_TYPE_DATA16_UNSIGNED",
	"ARG_TYPE_DATA16_UNSIGNED_INDIRECT",
	"ARG_TYPE_REL8",
	"ARG_TYPE_REL8_ADD_SP",
	"ARG_TYPE_HL_INDIRECT_DEC",
	"ARG_TYPE_HL_INDIRECT_INC",
	"ARG_TYPE_00H",
	"ARG_TYPE_08H",
	"ARG_TYPE_10H",
	"ARG_TYPE_18H",
	"ARG_TYPE_20H",
	"ARG_TYPE_28H",
	"ARG_TYPE_30H",
	"ARG_TYPE_38H",
	"ARG_TYPE_Z",
	"ARG_TYPE_NC",
	"ARG_TYPE_NZ",
	"ARG_TYPE_NONE"
};

#if 0
#define CPU_ERROR(state, arg0, i0, arg1, i1)\
	fprintf(stderr, "%s %s %d %s %d\n", __func__, ARG_TYPE_s[arg0], i0.r8, ARG_TYPE_s[arg1], i1.r16);\
	exit(-1)
#else
	#define CPU_ERROR(state, arg0, i0, arg1, i1)
#endif

#define ERR()\
	fprintf(stderr, "%s %s %d %s %d %d\n", __func__, ARG_TYPE_s[arg0], i0.r8, ARG_TYPE_s[arg1], i1.r16, __LINE__);\
	exit(-1)

/* CPU Helper functions */
reg_t   cpu_load_reg8(struct cpu_state *state, REG_INPUT reg)
{
	state->registers[reg.r8];
}

reg16_t cpu_load_reg16(struct cpu_state *state, REG_INPUT reg)
{
	state->registers16[reg.r16];
}

void cpu_store_reg8(struct cpu_state *state, REG_INPUT reg, reg_t data)
{
	state->registers[reg.r8] = data;
}

void cpu_store_reg16(struct cpu_state *state, REG_INPUT reg, reg16_t data)
{
	state->registers[reg.r16] = data;
}

/* Basic memory access functions */
reg_t cpu_load8(struct cpu_state *state, reg16_t addr)
{
	return state->memory[addr];
}
reg16_t cpu_load16(struct cpu_state *state, reg16_t addr)
{
	return *(reg16_t*)&state->memory[addr];
}

reg_t cpu_load8_indirect(struct cpu_state *state, REG_INPUT reg)
{
	reg16_t addr = state->registers16[reg.r16];
	return state->memory[addr];
}

reg16_t cpu_load16_indirect(struct cpu_state *state, REG_INPUT reg)
{
	reg16_t addr = state->registers16[reg.r16];
	return *(reg16_t*)&state->memory[addr];
}

void cpu_store8(struct cpu_state *state, reg16_t addr, reg_t data)
{
	state->memory[addr] = data;
}

void cpu_store16(struct cpu_state *state, reg16_t addr, reg16_t data)
{
	*(reg16_t*)&state->memory[addr] = data;
}

void cpu_store8_indirect(struct cpu_state *state, REG_INPUT reg, reg_t data)
{
	reg16_t addr = state->registers16[reg.r16];
	state->memory[addr] = data;
}

void cpu_store16_indirect(struct cpu_state *state, REG_INPUT reg, reg16_t data)
{
	reg16_t addr = state->registers16[reg.r16];
	*(reg16_t*)&state->memory[addr] = data;
}

void cpu_dec8(struct cpu_state *state, REG_INPUT reg)
{
	reg_t res                = state->registers[reg.r8] - 1;
	state->zero              = (res == 0);
	state->subtract          = 1;
	state->half_carry        = ((state->registers[reg.r8] & 0xf) != 0);
	state->registers[reg.r8] = res;
}

void cpu_inc8(struct cpu_state *state, REG_INPUT reg)
{
	int old_carry = state->carry;
	state->registers[reg.r8] = cpu_add8(state, state->registers[reg.r8], 1);
	state->carry             = old_carry;
}

void cpu_dec16(struct cpu_state *state, REG_INPUT reg)
{
	//No flags are affected with the 16bit decrement.
	state->registers16[reg.r16] -= 1;
}

void cpu_inc16(struct cpu_state *state, REG_INPUT reg)
{
	//No flags are affected with the 16bit increment.
	state->registers16[reg.r16] += 1;
}

reg_t cpu_and(struct cpu_state *state, reg_t d0, reg_t d1)
{
	reg_t res         = d0 & d1;
	state->zero       = res == 0;
	state->half_carry = 0;
	state->carry      = 0;
	state->subtract   = 0;
	return res;
}

reg_t cpu_or(struct cpu_state *state, reg_t d0, reg_t d1)
{
	reg_t res         = d0 | d1;
	state->zero       = res == 0;
	state->half_carry = 0;
	state->carry      = 0;
	state->subtract   = 0;
	return res;
}

reg_t cpu_adc(struct cpu_state *state, reg_t d0, reg_t d1)
{
	uint16_t res      = ((uint16_t)d0 + d1) + state->carry;
	state->zero       = (d0 & 0xff) == 0;
	state->subtract   = 0;
	state->half_carry = ((d0 & 0xf) + (d1 & 0xf) + state->carry) > 0xf;
	state->carry      = res > 0xff;
	return res & 0xff;
}

reg_t cpu_add8(struct cpu_state *state, reg_t d0, reg_t d1)
{
	uint16_t t0       = (uint16_t)d0 + (uint16_t)d1;
	state->zero       = (t0 & 0xff) == 0;
	state->subtract   = 0;
	state->half_carry = (d0 & 0xf) + (d1 & 0xf) > 0xf;
	state->carry      = t0 > 0xff;
	return t0 & 0xff;
}

reg_t cpu_add16(struct cpu_state *state, reg16_t d0, reg16_t d1)
{
	uint32_t t0       = (uint32_t)d0 + (uint32_t)d1;
	state->zero       = (t0 & 0xffff) == 0;
	state->subtract   = 0;
	state->carry      = t0 > 0xffff;
	state->half_carry = (d0 & 0xfff) + (d1 & 0xfff) > 0xfff;
	return t0 & 0xffff;
}

void  cpu_push(struct cpu_state *state, reg16_t d0)
{
	state->sp -= 2;
	*(reg16_t*)&state->memory[state->sp] = d0;
}

void cpu_pop(struct cpu_state *state, REG_INPUT reg)
{
	state->registers16[reg.r16] = *(reg16_t*)&state->memory[state->sp];
	state->sp += 2;
}

void  cpu_jump(struct cpu_state *state, reg16_t addr)
{
	state->pc = addr;
}

void  cpu_jump_rel(struct cpu_state *state, reg_t addr)
{
	state->pc += addr;
}

void cpu_ret(struct cpu_state *state)
{
	REG_INPUT t0 = {.r16 = REG16_SP};
	cpu_pop(state, t0);
}

void cpu_reti(struct cpu_state *state)
{
	REG_INPUT t0 = {.r16 = REG16_SP};
	cpu_pop(state, t0);
	cpu_enable_interrupts(state);
}

void cpu_enable_interrupts(struct cpu_state *state)
{
	//TODO:Implement
}

void cpu_disable_interrupts(struct cpu_state *state)
{
	//TODO:Implement
}

void cpu_cmp(struct cpu_state *state, reg_t d0)
{
	state->carry      = state->a < d0;
	state->zero       = state->a == d0;
	state->half_carry = (state->a & 0xf) < (d0 & 0xf);
	state->subtract   = 1;
}

int cpu_carry(struct cpu_state *state)
{
	return state->carry;
}

int cpu_zero(struct cpu_state *state)
{
	return state->zero;
}

void cpu_xor(struct cpu_state *state, reg_t d0)
{
	state->a ^= d0;
	state->zero       = (state->a == 0);
	state->half_carry = 0;
	state->carry      = 0;
	state->subtract   = 0;
}

void cpu_sbc(struct cpu_state *state, reg_t d0)
{
	reg16_t t0        = d0 + state->carry;
	state->carry      = state->a < t0;
	state->half_carry = (state->a & 0xf) < (t0 & 0xf);
	state->a          = (state->a - (reg_t)t0);
	state->zero       = (state->a == 0);
}

void cpu_sub(struct cpu_state *state, reg_t d0)
{
	int16_t res = state->a - d0;
	if(res < 0)
	{
		res += 0xff;
	}
}

void cpu_rst(struct cpu_state *state, reg_t d0)
{
	cpu_push(state, state->pc);
	state->pc = d0;
}


//TODO:Check if these actually set the zero flag.
void cpu_rla(struct cpu_state *state)
{
	state->a = cpu_rl(state, state->a);
}

void cpu_rlca(struct cpu_state *state)
{
	state->a = cpu_rlc(state, state->a);
}

void cpu_rra(struct cpu_state *state)
{
	state->a = cpu_rr(state, state->a);
}

/* Rotate A right, store bit 0 in carry flag */
void cpu_rrca(struct cpu_state *state)
{
	state->a = cpu_rrc(state, state->a);
}

void cpu_bit(struct cpu_state *state, reg_t pos, reg_t d0)
{
	state->zero = !((d0 >> pos) & 0x1);
	state->subtract = 0;
	state->half_carry = 1;
}

reg_t cpu_res(struct cpu_state *state, reg_t pos, reg_t d0)
{
	reg_t mask = ~((reg_t)1 << pos);
	return d0 & mask;
}

reg_t cpu_set(struct cpu_state *state, reg_t pos, reg_t d0)
{
	return d0 | 1 << pos;
}

reg_t   cpu_rl(struct cpu_state *state, reg_t d0)
{
	reg_t old_carry = state->carry;
	state->carry = (d0 >> 7) & 0x1;
	d0 =  (d0 << 1) | old_carry;
	state->zero       = (d0 == 0);
	state->half_carry = 0;
	state->subtract   = 0;
	return d0;
}

reg_t   cpu_rlc(struct cpu_state *state, reg_t d0)
{
	state->carry = (d0 >> 7) & 0x1;
	d0 =  (d0 << 1) | state->carry;
	state->zero       = (d0 == 0);
	state->half_carry = 0;
	state->subtract   = 0;
	return d0;
}

reg_t   cpu_rr(struct cpu_state *state, reg_t d0)
{
	reg_t old_carry = state->carry;
	state->carry = d0 & 0x1;
	d0 = (d0 >> 1) | (old_carry << 7);
	state->zero       = (d0 == 0);
	state->half_carry = 0;
	state->subtract   = 0;
	return d0;
}

reg_t   cpu_rrc(struct cpu_state *state, reg_t d0)
{
	state->carry = d0 & 0x1;
	d0 = (d0 >> 1) | (state->carry << 7);
	state->zero       = (d0 == 0);
	state->half_carry = 0;
	state->subtract   = 0;
	return d0;
}

reg_t   cpu_sra(struct cpu_state *state, reg_t d0)
{
	state->carry = d0 & 0x1;
	d0 = (d0 >> 1) | ((d0 >> 7) << 7);
	state->zero       = (d0 == 0);
	state->half_carry = 0;
	state->subtract   = 0;
	return d0;
}

reg_t   cpu_sla(struct cpu_state *state, reg_t d0)
{
	state->carry = (d0 >> 7) & 0x1;
	d0 <<= 1;
	state->zero       = (d0 == 0);
	state->half_carry = 0;
	state->subtract   = 0;
	return d0;
}

reg_t   cpu_srl(struct cpu_state *state, reg_t d0)
{
	state->carry = d0 & 0x1;
	d0 >>= 1;
	state->zero       = (d0 == 0);
	state->half_carry = 0;
	state->subtract   = 0;
	return d0;
}

reg_t cpu_swap(struct cpu_state *state, reg_t d0)
{
	return (d0 << 4) & (d0 >> 4);
}
