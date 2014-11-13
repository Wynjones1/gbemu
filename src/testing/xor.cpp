#include "testing.h"

void xor_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0xff, 0xff, 0x00, 1, 0, 0, 0},
		{0xff, 0x0f, 0xf0, 0, 0, 0, 0},
		{0xff, 0x8a, 0x75, 0, 0, 0, 0},
	};
	for(unsigned int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.half_carry = rand() % 2;
		state.subtract   = rand() % 2;
		state.carry      = rand() % 2;
		state.a = test[0];
		cpu_xor(&state, test[1]);
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
			printf("%d %d %d %d\n",  (int) state.zero,
									 (int) state.half_carry,
									 (int) state.subtract,
									 (int) state.carry);
		}
	}
}
