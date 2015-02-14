#include "testing.h"

void srl_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x01, 0,  0x00, 1, 0, 0, 1},
		{0xff, 0,  0x7f, 0, 0, 0, 1},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test             = tests[i];
		cpu_set_carry(&state, test[1]);
		cpu_set_half_carry(&state, rand() % 2);
		cpu_set_subtract(&state, rand() % 2);
		cpu_set_zero(&state, rand() % 2);
		reg_t res = cpu_srl(&state, test[0]);
		if(res              == test[2] &&
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
