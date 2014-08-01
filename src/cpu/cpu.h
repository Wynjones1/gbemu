#pragma once
#include <stdint.h>

typedef uint8_t  reg_t;
typedef uint16_t reg16_t;

enum REG
{
	REG_F,REG_A, 
	REG_C,REG_B, 
	REG_E,REG_D, 
	REG_L,REG_H, 
	NUM_REGISTERS
};

enum REG16
{
	REG16_AF,
	REG16_BC,
	REG16_DE,
	REG16_HL,
	REG16_SP,
	NUM_REGISTERS16
};

enum ARG_TYPE
{
	ARG_TYPE_REG8,
	ARG_TYPE_REG8_INDIRECT,
	ARG_TYPE_REG16,
	ARG_TYPE_REG16_INDIRECT,
	ARG_TYPE_DATA8,
	ARG_TYPE_DATA8_UNSIGNED,
	ARG_TYPE_DATA8_UNSIGNED_INDIRECT,
	ARG_TYPE_DATA16,
	ARG_TYPE_DATA16_UNSIGNED,
	ARG_TYPE_DATA16_UNSIGNED_INDIRECT,
	ARG_TYPE_REL8,
	ARG_TYPE_REL8_ADD_SP,
	ARG_TYPE_HL_INDIRECT_DEC,
	ARG_TYPE_HL_INDIRECT_INC,
	ARG_TYPE_00H,
	ARG_TYPE_08H,
	ARG_TYPE_10H,
	ARG_TYPE_18H,
	ARG_TYPE_20H,
	ARG_TYPE_28H,
	ARG_TYPE_30H,
	ARG_TYPE_38H,
	ARG_TYPE_Z,
	ARG_TYPE_NC,
	ARG_TYPE_NZ,
	ARG_TYPE_NONE
};


typedef union REG_INPUT
{
	enum REG   r8;
	enum REG16 r16;
}REG_INPUT;


struct cpu_state
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
	reg16_t sp;
	reg16_t pc;
	reg_t   memory[65536];
};

typedef void (*cpu_opcode)(
				struct cpu_state *state,
				enum  ARG_TYPE    arg0,
				union REG_INPUT   i0,
				enum  ARG_TYPE    arg1,
				union REG_INPUT   i1);

struct opcode
{
	cpu_opcode       op;
	enum  ARG_TYPE   arg0;
	union REG_INPUT  i0;
	enum  ARG_TYPE   arg1;
	union REG_INPUT  i1;
};

extern struct opcode op_table[];

void NOP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void STOP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void JR( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void INC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void DEC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void ADD( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void LD( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void LDH( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void RST( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void PUSH( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void POP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void CP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void OR( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void CALL( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void SBC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void AND( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void JP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void ADC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void XOR( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void SUB( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void RET( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void RET_Z( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void PREFIX_CB( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);
void INVALID( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1);

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

void cpu_dec(struct cpu_state *state, REG_INPUT reg);
void cpu_inc(struct cpu_state *state, REG_INPUT reg);
