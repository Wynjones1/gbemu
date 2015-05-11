#include "cpu.h"

void DAA(cpu_state_t *state,
		enum ARG_TYPE arg0, union REG_INPUT i0,
		enum ARG_TYPE arg1, union REG_INPUT i1)
{
    reg16_t acc  = state->a;
    if(cpu_subtract(state))
    {
        if(cpu_half_carry(state))
        {
            acc -= 6;
            acc &= 0xff;
        }
        if(cpu_carry(state))
        {
            acc -= 0x60;
        }
    }
    else
    {
        if(!cpu_half_carry(state))
        {
            if((acc & 0x0f) > 0x09)
            {
                acc += 0x06;
            }
        }
        else
        {
            acc += 6;
        }

        if(!cpu_carry(state))
        {
            if(acc > 0x9f)
            {
                acc += 0x60;
            }
        }
        else
        {
            acc += 0x60;
        }
    }
    cpu_set_half_carry(state, 0);
    state->a = acc & 0xff;
    if((acc & 0x100) == 0x100)
    {
        cpu_set_carry(state, 1);
    }
    cpu_set_zero(state, state->a == 0);
}
