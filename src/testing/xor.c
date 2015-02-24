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
	for(uint32_t i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		cpu_set_half_carry(&state, rand() % 2);
		cpu_set_subtract(&state, rand() % 2);
		cpu_set_carry(&state, rand() % 2);
		state.a = test[0];
		cpu_xor(&state, test[1]);
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
			printf("%d %d %d %d\n",  (int) cpu_zero(&state),
									 (int) cpu_half_carry(&state),
									 (int) cpu_subtract(&state),
									 (int) cpu_carry(&state));
		}
	}
}
