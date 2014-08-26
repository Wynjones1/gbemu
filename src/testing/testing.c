#include "common.h"
#include "cpu.h"
#include <stdint.h>

int adc_test(void)
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
	return 0;
}

void temp(void)
{
	cpu_state_t state;
	memset(&state, 0x00, sizeof(state));
	state.carry = 1;
	uint8_t result = cpu_adc(&state, 0, 0);
	printf("result     %d\n", result);
	printf("zero       %d\n", state.zero);
	printf("carry      %d\n", state.carry);
	printf("half carry %d\n", state.half_carry);
	printf("subtract   %d\n", state.subtract);
}

int flag_test(void)
{
	cpu_state_t state;
	memset(&state, 0x00, sizeof(state));
	state.f = 0;
	state.carry = 1;
	state.half_carry = 1;
	state.subtract = 1;
	printf("%02x\n", state.f);
	return 0;
}

int testing(int argc, char **argv)
{
	//flag_test();
	temp();
	//adc_test();
	return 0;
}
