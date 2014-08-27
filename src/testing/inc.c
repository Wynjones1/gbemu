#include "testing.h"

void inc_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][6] =
	{
		{0x00, 0x01, 0, 0, 0, 0},
		{0x0f, 0x10, 0, 1, 0, 0},
		{0xff, 0x00, 1, 1, 0, 0},
		{0xf0, 0xf1, 0, 0, 0, 0},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		uint8_t res = cpu_inc8(&state, test[0]);
		state.carry    = test[5];
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
			printf("%d %d %d %d\n",  (int) state.zero,
									 (int) state.half_carry,
									 (int) state.subtract,
									 (int) state.carry);
		}
	}
}
