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
	for(uint32_t i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test             = tests[i];
		state.a          = test[0];
		cpu_set_carry(&state, test[1]);
		cpu_set_half_carry(&state, rand() % 2);
		cpu_set_subtract(&state, rand() % 2);
		cpu_set_zero(&state, rand() % 2);
		cpu_rra(&state);
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
			printf("0x%02x %d %d %d %d\n",
									 (int) state.a,
									 (int) cpu_zero(&state),
									 (int) cpu_half_carry(&state),
									 (int) cpu_subtract(&state),
									 (int) cpu_carry(&state));
		}
	}
}
