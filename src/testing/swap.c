#include "testing.h"

void swap_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x00, 0x00, 1, 0, 0, 0},
		{0xf0, 0x0f, 0, 0, 0, 0},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		uint8_t res = cpu_swap(&state, test[0]);
		if(res              == test[1] &&
		   cpu_zero(&state)       == test[2] &&
		   cpu_half_carry(&state) == test[3] &&
		   cpu_subtract(&state)   == test[4] &&
		   cpu_carry(&state)      == test[5])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%02x %d %d %d %d\n",   (int) res,
											 (int) cpu_zero(&state),
											 (int) cpu_half_carry(&state),
											 (int) cpu_subtract(&state),
											 (int) cpu_carry(&state));
		}
	}
}
