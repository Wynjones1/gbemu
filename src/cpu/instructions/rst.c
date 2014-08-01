#include "cpu.h"

void RST(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_00H)
	{
		cpu_rst(state,0x00);
	}
	else if(arg0 == ARG_TYPE_08H)
	{
		cpu_rst(state,0x08);
	}
	else if(arg0 == ARG_TYPE_10H)
	{
		cpu_rst(state,0x10);
	}
	else if(arg0 == ARG_TYPE_18H)
	{
		cpu_rst(state,0x18);
	}
	else if(arg0 == ARG_TYPE_20H)
	{
		cpu_rst(state,0x20);
	}
	else if(arg0 == ARG_TYPE_28H)
	{
		cpu_rst(state,0x28);
	}
	else if(arg0 == ARG_TYPE_30H)
	{
		cpu_rst(state,0x30);
	}
	else if(arg0 == ARG_TYPE_38H)
	{
		cpu_rst(state,0x38);
	}
}
