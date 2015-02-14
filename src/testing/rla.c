#include "testing.h"

void rla_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x95, 1,  0x2b, 0, 0, 0, 1},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test             = tests[i];
		state.a          = test[0];
		cpu_set_carry(&state, test[1]);
		cpu_set_half_carry(&state, rand() % 2);
		cpu_set_subtract(&state, rand() % 2);
		cpu_set_zero(&state, rand() % 2);
		cpu_rla(&state);
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
			printf("0x%02x %d %d %d %d\n",
									 (int) state.a,
									 (int) state.zero,
									 (int) state.half_carry,
									 (int) state.subtract,
									 (int) state.carry);
		}
	}
}
