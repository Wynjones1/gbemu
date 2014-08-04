#include "cpu.h"

void DAA(struct cpu_state *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
	//TODO: Make sure this is correct.
	state->half_carry = 0;
	reg_t hbits = (state->a & 0xf0) >> 4;
	reg_t lbits = (state->a & 0x0f);
	if(state->subtract)
	{
		if(state->carry)
		{
			if( hbits >= 0x7 && hbits <= 0xf &&
				lbits >= 0x0 && lbits <= 0x9 && !state->half_carry)
			{
				state->a += 0xA0;
				state->carry = 1;
			}
			else if( hbits >= 0x6 && hbits <= 0xf &&
				lbits >= 0x6 && lbits <= 0xf && state->half_carry)
			{
				state->a += 0x9A;
				state->carry = 1;
			}
		}
		else
		{
			if( hbits >= 0x0 && hbits <= 0x9 &&
				lbits >= 0x0 && lbits <= 0x9 && !state->half_carry)
			{
				state->a += 0x00;
				state->carry = 0;
			}
			else if( hbits >= 0x0 && hbits <= 0x8 &&
				lbits >= 0x6 && lbits <= 0xf && state->half_carry)
			{
				state->a += 0xFA;
				state->carry = 0;
			}
		}
	}
	else
	{
		if(state->carry)
		{
			if( hbits >= 0x0 && hbits <= 0x3 &&
				lbits >= 0x0 && lbits <= 0x3 && state->half_carry)
			{
				state->a += 0x66;
				state->carry = 1;
			}
			else if( hbits >= 0x0 && hbits <= 0x2 &&
				lbits >= 0xa && lbits <= 0xf && !state->half_carry)
			{
				state->a += 0x66;
				state->carry = 1;
			}
			else if( hbits >= 0x0 && hbits <= 0x2 &&
				lbits >= 0x0 && lbits <= 0x9 && !state->half_carry)
			{
				state->a += 0x60;
				state->carry = 1;
			}
		}
		else
		{
			if( hbits >= 0xa && hbits <= 0xf &&
				lbits >= 0x0 && lbits <= 0x3 && state->half_carry)
			{
				state->a += 0x66;
				state->carry = 1;
			}
			else if( hbits >= 0x9 && hbits <= 0xf &&
				lbits >= 0xa && lbits <= 0xf && !state->half_carry)
			{
				state->a += 0x66;
				state->carry = 1;
			}
			else if( hbits >= 0xa && hbits <= 0xf &&
				lbits >= 0x0 && lbits <= 0x9 && !state->half_carry)
			{
				state->a += 0x60;
				state->carry = 1;
			}
			else if( hbits >= 0x0 && hbits <= 0x9 &&
				lbits >= 0x0 && lbits <= 0x3 && state->half_carry)
			{
				state->a += 0x06;
				state->carry = 0;
			}
			else if( hbits >= 0x0 && hbits <= 0x8 &&
				lbits >= 0xa && lbits <= 0xf && !state->half_carry)
			{
				state->a += 0x06;
				state->carry = 0;
			}
			else if( hbits >= 0x0 && hbits <= 0x9 &&
				lbits >= 0x0 && lbits <= 0x9 && !state->half_carry)
			{
				state->a += 0x00;
				state->carry = 0;
			}
		}
	}
}
