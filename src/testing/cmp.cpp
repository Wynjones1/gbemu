#include "testing.h"

void cmp_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][6] =
	{
		{0x3c, 0x2f, 0, 1, 1, 0},
		{0x3c, 0x3c, 1, 0, 1, 0},
		{0x3c, 0x40, 0, 0, 1, 1},
	};
	for(unsigned int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.a = test[0];
		cpu_cmp(&state, test[1]);
		if(state.zero       == test[2] &&
		   state.half_carry == test[3] &&
		   state.subtract   == test[4] &&
		   state.carry      == test[5])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
		}
	}
}
