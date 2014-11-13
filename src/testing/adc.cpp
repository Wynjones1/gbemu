#include "testing.h"

void adc_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][8] =
	{
		{0xe1, 0x0f, 0x1, 0xf1, 0x0, 0x1, 0x0, 0x0},
		{0xe1, 0x3b, 0x1, 0x1d, 0x0, 0x0, 0x0, 0x1},
		{0xe1, 0x1e, 0x1, 0x00, 0x1, 0x1, 0x0, 0x1},
	};
	for(unsigned int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.half_carry = rand() % 2;
		state.subtract   = rand() % 2;
		state.carry      = test[2];
		reg_t res = cpu_adc(&state, test[0], test[1]);
		if(res              == test[3] &&
		   state.zero       == test[4] &&
		   state.half_carry == test[5] &&
		   state.subtract   == test[6] &&
		   state.carry      == test[7])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%02x %d %d %d %d\n",  res, (int) state.zero,
									 (int) state.half_carry,
									 (int) state.subtract,
									 (int) state.carry);
		}
	}
}
