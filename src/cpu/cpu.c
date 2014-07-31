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

/* Write a 16 bit value into a 16 bit register */
static void cpu_write_reg16(struct cpu_state *state, int reg16, reg16_t data)
{
}

/* Write a 8 bit value into a 8 bit register */
static void cpu_write_reg8(struct cpu_state *state, int reg8, reg_t data)
{
}

/* Store 8 bit value into memory */
static void cpu_store(struct cpu_state *state, reg16_t loc, reg_t data)
{
}


/* Store 16 bit value into memory */
static void cpu_store_16(struct cpu_state *state, reg16_t loc, reg16_t data)
{
}

/* Load 8 bit value from memory */
static reg_t cpu_load(struct cpu_state *state, reg16_t loc)
{
}

/* Load 16 bit value from memory */
static reg16_t cpu_load_16(struct cpu_state *state, reg16_t loc)
{
}

/* Add two values together (sets the appropriate flags in state) */
static reg16_t cpu_add16(struct cpu_state *state, reg16_t a0, reg16_t a1)
{
}

/* Add two values together (sets the appropriate flags in state) */
static reg_t cpu_add8(struct cpu_state *state, reg_t a0, reg_t a1)
{
}

/* Subtract two values together (sets the appropriate flags in state) */
static reg16_t cpu_sub16(struct cpu_state *state, reg16_t a0, reg16_t a1)
{
}

/* Subtract two values together (sets the appropriate flags in state) */
static reg_t cpu_sub8(struct cpu_state *state, reg_t a0, reg_t a1)
{
}

static reg_t cpu_get_reg8(struct cpu_state *state, int r8)
{
	return state->registers[r8];
}

static reg_t cpu_get_reg16(struct cpu_state *state, int r16)
{
	return state->registers[r16];
}

static void cpu_inc(struct cpu_state *state, int r16)
{
	
}

static void cpu_dec(struct cpu_state *state, int r16)
{
	
}

/* CPU Helper macros */
#define ARGS_MATCH(t0, t1) (arg0 == ARG_TYPE_ ## t0) && (arg1 == ARG_TYPE_ ## t1)
#define FIRST_ARG8(state)           state->memory[state->sp + 1]
#define SECOND_ARG8(state, offset)  state->memory[state->sp + 1 + offset]
#define FIRST_ARG16(state)          *(reg16_t*)(&state->memory[state->sp + 1])
#define SECOND_ARG16(state, offset) *(reg16_t*)(&state->memory[state->sp + 1 + offset])


/* CPU Instructions */
// Each of these function have the same signature, the first being the cpu state for the emulator.
// The second argument describes the type of the argument, if the argument is a register type then
// the third argument containes the index to the register that it references, the same is true for
// the fourth and fifth argument. If the argument is a immediate argument type then the data can
// be found with the FIRST/SECOND_ARG macro.
/*

void NOP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	//CPU_ERROR(state,arg0, i0, arg1, i1);
}
void STOP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void JR( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void INC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(ARGS_MATCH(REG16, NONE))
	{
		reg16_t d = cpu_get_reg16(state, i0.r16);
		d = cpu_add16(state, d, 1);
		cpu_write_reg16(state, i0.r16, d);
	}
	else if(ARGS_MATCH(REG8, NONE))
	{
		reg_t d = cpu_get_reg8(state, i0.r8);
		d = cpu_add8(state, d, 1);
		cpu_write_reg16(state, i0.r8, d);
	}
	else
	{
		CPU_ERROR(state,arg0, i0, arg1, i1);
	}
}
void DEC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(ARGS_MATCH(REG16, NONE))
	{
		reg16_t d = cpu_get_reg16(state, i0.r16);
		d = cpu_sub16(state, d, 1);
		cpu_write_reg16(state, i0.r16, d);
	}
	else if(ARGS_MATCH(REG8, NONE))
	{
		reg_t d = cpu_get_reg8(state, i0.r8);
		d = cpu_sub8(state, d, 1);
		cpu_write_reg16(state, i0.r8, d);
	}
	else
	{
		CPU_ERROR(state,arg0, i0, arg1, i1);
	}
}
void ADD( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(ARGS_MATCH(REG16, REG16))
	{
		reg16_t a0 = cpu_get_reg16(state, i0.r16);
		reg16_t a1 = cpu_get_reg16(state, i1.r16);
		a0 = cpu_add16(state, a0, a1);
		cpu_write_reg16(state, i0.r16, a0);
	}
	else if(ARGS_MATCH(REG8, REG8))
	{
		reg_t a0 = cpu_get_reg8(state, i0.r8);
		reg_t a1 = cpu_get_reg8(state, i1.r8);
		a0 = cpu_add8(state, a0, a1);
		cpu_write_reg8(state, i0.r8, a0);
	}
	else if(ARGS_MATCH(REG8, REG16_INDIRECT))
	{
		reg16_t l = cpu_get_reg16(state, i1.r16);
		reg_t a0 = cpu_get_reg8(state, i0.r8);
		reg_t a1 = cpu_load_16(state, l);
		a0 = cpu_add8(state, a0, a1);
		cpu_write_reg8(state, i0.r8, a0);
	}
	else if(ARGS_MATCH(REG8, DATA8))
	{
		reg_t a0 = cpu_get_reg8(state, i0.r8);
		reg_t a1 = FIRST_ARG8(state);
		a0 = cpu_add8(state, a0, a1);
		cpu_write_reg8(state, i0.r8, a0);
	}
	else
	{
		ERR();
	}
	CPU_ERROR(state,arg0, i0, arg1, i1);
}

void LD( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(ARGS_MATCH(REG16, DATA16))
	{
		reg16_t d = FIRST_ARG16(state);
		cpu_write_reg16(state, i0.r16, d);
	}
	else if(ARGS_MATCH(REG16_INDIRECT, REG8))
	{
		reg_t   r = cpu_get_reg8(state, i1.r8);
		reg16_t l = FIRST_ARG16(state);
		cpu_store(state, l, r);
	}
	else if(ARGS_MATCH(REG8, DATA8))
	{
		reg_t d = FIRST_ARG8(state);
		cpu_write_reg8(state, i0.r8, d);
	}
	else if(ARGS_MATCH(DATA16_UNSIGNED_INDIRECT, REG16))
	{
		reg16_t r = cpu_get_reg16(state, i1.r16);
		reg16_t l = FIRST_ARG16(state);
		cpu_store_16(state, l, r);
	}
	else if(ARGS_MATCH(REG8, REG16_INDIRECT))
	{
		reg16_t l = cpu_get_reg16(state, i1.r16);
		reg_t   d = cpu_load(state, l);
		cpu_write_reg8(state, i0.r8, d);
	}
	else if(ARGS_MATCH(HL_INDIRECT_INC, REG8))
	{
		reg_t   r = cpu_get_reg8(state, i1.r8);
		reg16_t l = FIRST_ARG16(state);
		cpu_store(state, l, r);

		cpu_inc(state, i0.r16);
	}
	else if(ARGS_MATCH(REG8, HL_INDIRECT_INC))
	{
		reg16_t l = cpu_get_reg16(state, i1.r16);
		reg_t   d = cpu_load(state, l);
		cpu_write_reg8(state, i0.r8, d);

		cpu_inc(state, i1.r16);
	}
	else if(ARGS_MATCH(HL_INDIRECT_DEC, REG8))
	{
		reg_t   r = cpu_get_reg8(state, i1.r8);
		reg16_t l = FIRST_ARG16(state);
		cpu_store(state, l, r);

		cpu_dec(state, i0.r16);
	}
	else if(ARGS_MATCH(REG16_INDIRECT, DATA8))
	{
		reg16_t l = FIRST_ARG8(state);
		cpu_store(state, l, i1.r8);
	}
	else if(ARGS_MATCH(REG8, HL_INDIRECT_DEC))
	{
		reg16_t l = cpu_get_reg16(state, i1.r16);
		reg_t   d = cpu_load(state, l);
		cpu_write_reg8(state, i0.r8, d);

		cpu_dec(state, i1.r16);
	}
	else if(ARGS_MATCH(REG8, REG8))
	{
		reg_t d = cpu_get_reg8(state, i1.r8);
		cpu_write_reg8(state, i0.r8, d);
	}
	else if(ARGS_MATCH(REG8_INDIRECT, REG8))
	{
		reg16_t l = cpu_get_reg8(state, i0.r8);
		reg_t   d = cpu_get_reg8(state, i1.r8);

		l += 0xff00; //TODO: Change to cpu_add.
		cpu_store(state, l, d);
	}
	else if(ARGS_MATCH(DATA16_UNSIGNED_INDIRECT, REG8))
	{
		reg16_t l = FIRST_ARG16(state);
		reg_t   d = cpu_get_reg8(state, i1.r8);
		cpu_store(state, l, d);
	}
	else if(ARGS_MATCH(REG8, REG8_INDIRECT))
	{
		reg16_t l = cpu_get_reg8(state, i1.r8);
		l += 0xff00;
		reg_t d = cpu_load(state, l);
		cpu_write_reg8(state, i0.r8, d);
	}
	else if(ARGS_MATCH(REG16, REL8_ADD_SP))
	{
		reg16_t l = cpu_get_reg8(state, i1.r8);
		//TODO: Change to the cpu addition function (when it exists...)
		l += cpu_get_reg16(state, REG16_SP);
		reg16_t d = cpu_load(state, l);
		cpu_write_reg16(state, i0.r16, d);
	}
	else if(ARGS_MATCH(REG16, REG16))
	{
		reg16_t d = cpu_get_reg16(state, i1.r16);
		cpu_write_reg16(state, i0.r16, d);
	}
	else if(ARGS_MATCH(REG8, DATA16_UNSIGNED_INDIRECT))
	{
		reg16_t l = FIRST_ARG16(state);
		reg_t d = cpu_load(state, l);
		cpu_write_reg8(state, i0.r8, d);
	}
	else
	{
		CPU_ERROR(state,arg0, i0, arg1, i1);
	}
}
void LDH( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void RST( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void PUSH( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void POP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void CP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void OR( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void CALL( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void SBC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void AND( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void JP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void ADC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void XOR( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void SUB( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void RET( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void RET_Z( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void PREFIX_CB( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void INVALID( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	//TODO: Add error checking for if this is actually called.
	//CPU_ERROR(state,arg0, i0, arg1, i1);
}
*/
