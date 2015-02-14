#include "testing.h"

void bit_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][6] =
	{
		{7, 0x80, 0, 1, 0, 0},
		{4, 0xef, 1, 1, 0, 0},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		cpu_bit(&state, test[0], test[1]);
		cpu_set_carry(&state, test[5]);
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
			printf("%d %d %d %d\n",  (int) state.zero,
									 (int) state.half_carry,
									 (int) state.subtract,
									 (int) state.carry);
		}
	}
}
