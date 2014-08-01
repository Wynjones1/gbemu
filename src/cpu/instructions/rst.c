#include "cpu.h"

void RST(struct cpu_state *state,
		const enum ARG_TYPE arg0, const union REG_INPUT i0,
		const enum ARG_TYPE arg1, const union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_00H)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
	else if(arg0 == ARG_TYPE_08H)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
	else if(arg0 == ARG_TYPE_10H)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
	else if(arg0 == ARG_TYPE_18H)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
	else if(arg0 == ARG_TYPE_20H)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
	else if(arg0 == ARG_TYPE_28H)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
	else if(arg0 == ARG_TYPE_30H)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
	else if(arg0 == ARG_TYPE_38H)
	{
		if(arg1 == ARG_TYPE_NONE)
		{
		}
	}
}