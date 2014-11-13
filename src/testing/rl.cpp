#include "testing.h"

void rl_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x80, 0,  0x00, 1, 0, 0, 1},
		{0x11, 0,  0x22, 0, 0, 0, 0},
	};
	for(unsigned int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test             = tests[i];
		state.carry      = test[1];
		state.half_carry = rand() % 2;
		state.subtract   = rand() % 2;
		state.zero       = rand() % 2;
		reg_t res = cpu_rl(&state, test[0]);
		if(res              == test[2] &&
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
