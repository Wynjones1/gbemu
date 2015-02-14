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
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.a = test[0];
		cpu_sub(&state, test[1]);
		if(state.a          == test[2] &&
		   cpu_zero(&state)       == test[3] &&
		   cpu_half_carry(&state) == test[4] &&
		   cpu_subtract(&state)   == test[5] &&
		   cpu_carry(&state)      == test[6])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%02x %d %d %d %d\n", (int) state.a,
											 (int) cpu_zero(&state),
											 (int) cpu_half_carry(&state),
											 (int) cpu_subtract(&state),
											 (int) cpu_carry(&state));
		}
	}
}
