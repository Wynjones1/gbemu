#include "cpu.h"

void SET(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_1)
	{
		if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_2)
	{
		if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_3)
	{
		if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_4)
	{
		if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_5)
	{
		if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_6)
	{
		if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_7)
	{
		if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_DATA8)
	{
		if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
}