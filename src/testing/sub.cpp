#include "testing.h"

void sub_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x3e, 0x3e, 0x00, 1, 0, 1, 0},
		{0x3e, 0x0f, 0x2f, 0, 1, 1, 0},
		{0x3e, 0x40, 0xfe, 0, 0, 1, 1},
	};
	for(unsigned int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.a = test[0];
		cpu_sub(&state, test[1]);
		if(state.a          == test[2] &&
		   state.zero       == test[3] &&
		   state.half_carry == test[4] &&
		   state.subtract   == test[5] &&
		   state.carry      == test[6])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%02x %d %d %d %d\n", (int) state.a,
											 (int) state.zero,
											 (int) state.half_carry,
											 (int) state.subtract,
											 (int) state.carry);
		}
	}
}
