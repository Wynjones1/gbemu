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

#define CPU_ERROR(state, arg0, i0, arg1, i1)\
	fprintf(stderr, "%s %s %d %s %d\n", __func__, ARG_TYPE_s[arg0], i0.r8, ARG_TYPE_s[arg1], i1.r16);\
	exit(-1)

void NOP( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
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
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void DEC( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void ADD( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
void LD( struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	CPU_ERROR(state,arg0, i0, arg1, i1);
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
	CPU_ERROR(state,arg0, i0, arg1, i1);
}
