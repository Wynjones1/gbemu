#include "testing.h"

void swap_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x00, 0x00, 1, 0, 0, 0},
		{0xf0, 0x0f, 0, 0, 0, 0},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		uint8_t res = cpu_swap(&state, test[0]);
		if(res              == test[1] &&
		   state.zero       == test[2] &&
		   state.half_carry == test[3] &&
		   state.subtract   == test[4] &&
		   state.carry      == test[5])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%02x %d %d %d %d\n",   (int) res,
											 (int) state.zero,
											 (int) state.half_carry,
											 (int) state.subtract,
											 (int) state.carry);
		}
	}
}
