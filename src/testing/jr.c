#include "testing.h"

void jr_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x100, 0x00, 0x100, 0, 1, 0, 0},
		{0x100, -1, 0xff, 0, 0, 1, 0},
		{0x100, -2, 0xfe, 1, 0, 0, 0},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.pc         = test[0];
		cpu_set_zero(&state, test[3]);
		cpu_set_half_carry(&state, test[4]);
		cpu_set_subtract(&state, test[5]);
		cpu_set_carry(&state, test[6]);
		cpu_jump_rel(&state, test[1]);
		if(state.pc         == test[2] &&
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
			printf("0x%04x %d %d %d %d\n",   (int) state.pc,
											 (int) state.zero,
											 (int) state.half_carry,
											 (int) state.subtract,
											 (int) state.carry);
		}
	}
}
