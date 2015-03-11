#include "cpu.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits" 

void DAA(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	//TODO: Make sure this is correct.
	reg_t hbits = (state->a & 0xf0) >> 4;
	reg_t lbits = (state->a & 0x0f);
	if(cpu_subtract(state))
	{
		if(cpu_carry(state))
		{
			if( hbits >= 0x7 && hbits <= 0xf &&
				lbits >= 0x0 && lbits <= 0x9 && !cpu_half_carry(state))
			{
				state->a += 0xA0;
				cpu_set_carry(state, 1);
			}
			else if( hbits >= 0x6 && hbits <= 0xf &&
				lbits >= 0x6 && lbits <= 0xf && cpu_half_carry(state))
			{
				state->a += 0x9A;
				cpu_set_carry(state, 1);
			}
		}
		else
		{
			if( hbits >= 0x0 && hbits <= 0x9 &&
				lbits >= 0x0 && lbits <= 0x9 && !cpu_half_carry(state))
			{
				state->a += 0x00;
				cpu_set_carry(state, 0);
			}
			else if( hbits >= 0x0 && hbits <= 0x8 &&
				lbits >= 0x6 && lbits <= 0xf && cpu_half_carry(state))
			{
				state->a += 0xFA;
				cpu_set_carry(state, 0);
			}
		}
	}
	else
	{
		if(cpu_carry(state))
		{
			if( hbits >= 0x0 && hbits <= 0x3 &&
				lbits >= 0x0 && lbits <= 0x3 && cpu_half_carry(state))
			{
				state->a += 0x66;
				cpu_set_carry(state, 1);
			}
			else if( hbits >= 0x0 && hbits <= 0x2 &&
				lbits >= 0xa && lbits <= 0xf && !cpu_half_carry(state))
			{
				state->a += 0x66;
				cpu_set_carry(state, 1);
			}
			else if( hbits >= 0x0 && hbits <= 0x2 &&
				lbits >= 0x0 && lbits <= 0x9 && !cpu_half_carry(state))
			{
				state->a += 0x60;
				cpu_set_carry(state, 1);
			}
		}
		else
		{
			if( hbits >= 0xa && hbits <= 0xf &&
				lbits >= 0x0 && lbits <= 0x3 && cpu_half_carry(state))
			{
				state->a += 0x66;
				cpu_set_carry(state, 1);
			}
			else if( hbits >= 0x9 && hbits <= 0xf &&
				lbits >= 0xa && lbits <= 0xf && !cpu_half_carry(state))
			{
				state->a += 0x66;
				cpu_set_carry(state, 1);
			}
			else if( hbits >= 0xa && hbits <= 0xf &&
				lbits >= 0x0 && lbits <= 0x9 && !cpu_half_carry(state))
			{
				state->a += 0x60;
				cpu_set_carry(state, 1);
			}
			else if( hbits >= 0x0 && hbits <= 0x9 &&
				lbits >= 0x0 && lbits <= 0x3 && cpu_half_carry(state))
			{
				state->a += 0x06;
				cpu_set_carry(state, 0);
			}
			else if( hbits >= 0x0 && hbits <= 0x8 &&
				lbits >= 0xa && lbits <= 0xf && !cpu_half_carry(state))
			{
				state->a += 0x06;
				cpu_set_carry(state, 0);
			}
			else if( hbits >= 0x0 && hbits <= 0x9 &&
				lbits >= 0x0 && lbits <= 0x9 && !cpu_half_carry(state))
			{
				state->a += 0x00;
				cpu_set_carry(state, 0);
			}
		}
	}
	cpu_set_half_carry(state, 0);
	cpu_set_zero(state, (state->a == 0));
}

#pragma GCC diagnostic pop
