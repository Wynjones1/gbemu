#pragma once
#include "arg_defs.h"

/* Forward declare cpu_state struct */
typedef struct cpu_state cpu_state_t;

typedef void (*cpu_opcode)(
				cpu_state_t *state,
				enum  ARG_TYPE    arg0,
				union REG_INPUT   i0,
				enum  ARG_TYPE    arg1,
				union REG_INPUT   i1);

struct opcode
{
	const char      *name;
	cpu_opcode       op;
	enum  ARG_TYPE   arg0;
	union REG_INPUT  i0;
	enum  ARG_TYPE   arg1;
	union REG_INPUT  i1;
	char  size;    //Size of instruction in bytes
	char  success; //Cycles on success
	char  fail;    //Cycles on failure
};

extern struct opcode op_table[];
extern struct opcode cb_op_table[];

void NOP( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void STOP( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void JR( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void INC( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void DEC( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void ADD( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void LD( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void LDH( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RST( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void PUSH( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void POP( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void CP( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void OR( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void CALL( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SBC( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void AND( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void JP( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void ADC( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void XOR( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SUB( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RET( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RET_Z( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void PREFIX_CB( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void INVALID( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void EI( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void DI( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RETI( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void HALT( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void CCF( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SCF( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void CPL( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void DAA( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RRA( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RLA( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RRCA( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RLCA( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RLC( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RRC( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RL( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RR( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SLA( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SRA( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SWAP( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SRL( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void BIT( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RES( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SET( cpu_state_t *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
