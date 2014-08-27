#include "testing.h"

void adc_test(void)
{
	cpu_state_t state;
	memset(&state, 0x00, sizeof(state));
	for(int i = 0; i < 0x100; i++)
	{
		for(int j = 0; j < 0x100; j++)
		{
			for(int c = 0; c < 2; c++)
			{
				state.carry = c;
				uint8_t result = cpu_adc(&state, i, j);
				printf("0x%02x 0x%02x 0x%02x = 0x%02x %d %d %d %d\n",
												i, j, c, result,
												state.zero, state.subtract,
												state.carry, state.half_carry);
			}
		}
	}
}
