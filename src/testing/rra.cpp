#include "testing.h"

void rra_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x81, 0,  0x40, 0, 0, 0, 1},
		{0x81, 1,  0xc0, 0, 0, 0, 1},
	};
	for(unsigned int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test             = tests[i];
		state.a          = test[0];
		state.carry      = test[1];
		state.half_carry = rand() % 2;
		state.subtract   = rand() % 2;
		state.zero       = rand() % 2;
		cpu_rra(&state);
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
