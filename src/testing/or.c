#include "testing.h"

void or_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x00, 0x00, 0x00, 1, 0, 0, 0},
		{0x5a, 0x5a, 0x5a, 0, 0, 0, 0},
		{0x5a, 0x03, 0x5b, 0, 0, 0, 0},
		{0x5a, 0x0f, 0x5f, 0, 0, 0, 0},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		cpu_set_half_carry(&state, rand() % 2);
		cpu_set_subtract(&state, rand() % 2);
		cpu_set_carry(&state, rand() % 2);
		uint8_t res      = cpu_or(&state, test[0], test[1]);
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
			printf("%d %d %d %d\n",  (int) cpu_zero(&state),
									 (int) cpu_half_carry(&state),
									 (int) cpu_subtract(&state),
									 (int) cpu_carry(&state));
		}
	}
}
