#pragma once
#include "arg_defs.h"

typedef struct cpu_state cpu_state_t;

typedef void (*cpu_opcode)(
				struct cpu_state *state,
				enum  ARG_TYPE    arg0,
				union REG_INPUT   i0,
				enum  ARG_TYPE    arg1,
				union REG_INPUT   i1);

struct opcode
{
	cpu_opcode       op;
	const char      *name;
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

void NOP( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void STOP( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void JR( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void INC( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void DEC( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void ADD( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void LD( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void LDH( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RST( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void PUSH( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void POP( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void CP( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void OR( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void CALL( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SBC( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void AND( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void JP( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void ADC( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void XOR( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SUB( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RET( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RET_Z( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void PREFIX_CB( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void INVALID( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void EI( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void DI( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RETI( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void HALT( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void CCF( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SCF( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void CPL( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void DAA( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RRA( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RLA( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RRCA( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RLCA( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RLC( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RRC( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RL( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RR( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SLA( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SRA( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SWAP( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SRL( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void BIT( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void RES( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
void SET( struct cpu_state *state, const enum ARG_TYPE arg0, const union REG_INPUT i0, const enum ARG_TYPE arg1, const union REG_INPUT i1);
