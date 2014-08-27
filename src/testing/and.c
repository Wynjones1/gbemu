#include "testing.h"

void and_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x5a, 0x3f, 0x1a, 0, 1, 0, 0},
		{0x5a, 0x38, 0x18, 0, 1, 0, 0},
		{0x5a, 0x00, 0x00, 1, 1, 0, 0},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.half_carry = rand() % 2;
		state.subtract   = rand() % 2;
		state.carry      = rand() % 2;
		state.zero       = rand() % 2;
		uint8_t res      = cpu_and(&state, test[0], test[1]);
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
			printf("0x%02x %d %d %d %d\n", res,  (int) state.zero,
									 (int) state.half_carry,
									 (int) state.subtract,
									 (int) state.carry);
		}
	}
}
