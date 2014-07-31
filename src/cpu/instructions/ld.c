#include "cpu.h"

void LD(struct cpu_state *state, enum ARG_TYPE arg0, union REG_INPUT i0, enum ARG_TYPE arg1, union REG_INPUT i1)
{
	if(arg0 == ARG_TYPE_DATA16_UNSIGNED_INDIRECT)
	{
		if(arg1 == ARG_TYPE_REG16)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_HL_INDIRECT_DEC)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_HL_INDIRECT_INC)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_REG16)
	{
		if(arg1 == ARG_TYPE_DATA16)
		{
		}
		else if(arg1 == ARG_TYPE_REG16)
		{
		}
		else if(arg1 == ARG_TYPE_REL8_ADD_SP)
		{
		}
	}
	else if(arg0 == ARG_TYPE_REG16_INDIRECT)
	{
		if(arg1 == ARG_TYPE_DATA8)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
	else if(arg0 == ARG_TYPE_REG8)
	{
		if(arg1 == ARG_TYPE_DATA16_UNSIGNED_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_DATA8)
		{
		}
		else if(arg1 == ARG_TYPE_HL_INDIRECT_DEC)
		{
		}
		else if(arg1 == ARG_TYPE_HL_INDIRECT_INC)
		{
		}
		else if(arg1 == ARG_TYPE_REG16_INDIRECT)
		{
		}
		else if(arg1 == ARG_TYPE_REG8)
		{
		}
		else if(arg1 == ARG_TYPE_REG8_INDIRECT)
		{
		}
	}
	else if(arg0 == ARG_TYPE_REG8_INDIRECT)
	{
		if(arg1 == ARG_TYPE_REG8)
		{
		}
	}
}