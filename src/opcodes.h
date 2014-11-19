#pragma once
#include "arg_defs.h"

typedef struct cpu_state cpu_state_t;

typedef void (*cpu_opcode)(
				struct cpu_state *state,
				ARG_TYPE    arg0,
				REG_INPUT   i0,
				ARG_TYPE    arg1,
				REG_INPUT   i1);

typedef struct opcode
{
	cpu_opcode op;
	const char *name;
	ARG_TYPE   arg0;
	REG_INPUT  i0;
	ARG_TYPE   arg1;
	REG_INPUT  i1;
	char       size;    //Size of instruction in bytes
	char       success; //Cycles on success
	char       fail;    //Cycles on failure
	opcode( cpu_opcode op, const char *name, ARG_TYPE arg0, REG_INPUT i0, ARG_TYPE arg1,
			REG_INPUT  i1, char size, char success, char fail)
	: op(op)
	, name(name)
	, arg0(arg0)
	, i0(i0)
	, arg1(arg1)
	, i1(i1)
	, size(size)
	, success(success)
	, fail(fail)
	{}
}opcode_t;

extern const struct opcode op_table[];
extern const struct opcode cb_op_table[];

void NOP( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void STOP( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void JR( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void INC( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void DEC( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void ADD( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void LD( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void LDH( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RST( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void PUSH( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void POP( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void CP( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void OR( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void CALL( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void SBC( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void AND( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void JP( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void ADC( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void XOR( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void SUB( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RET( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RET_Z( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void PREFIX_CB( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void INVALID( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void EI( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void DI( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RETI( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void HALT( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void CCF( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void SCF( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void CPL( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void DAA( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RRA( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RLA( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RRCA( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RLCA( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RLC( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RRC( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RL( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RR( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void SLA( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void SRA( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void SWAP( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void SRL( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void BIT( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void RES( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
void SET( cpu_state *state, const  ARG_TYPE arg0, const  REG_INPUT i0, const  ARG_TYPE arg1, const  REG_INPUT i1);
