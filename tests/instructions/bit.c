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
	for(uint32_t i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		cpu_bit(&state, test[0], test[1]);
		cpu_set_carry(&state, test[5]);
		if(cpu_zero(&state)       == test[2] &&
		   cpu_half_carry(&state) == test[3] &&
		   cpu_subtract(&state)   == test[4] &&
		   cpu_carry(&state)      == test[5])
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
