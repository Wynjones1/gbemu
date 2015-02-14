#include "testing.h"

void adc_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][8] =
	{
		{0xe1, 0x0f, 0x1, 0xf1, 0x0, 0x1, 0x0, 0x0},
		{0xe1, 0x3b, 0x1, 0x1d, 0x0, 0x0, 0x0, 0x1},
		{0xe1, 0x1e, 0x1, 0x00, 0x1, 0x1, 0x0, 0x1},
	};
	for(int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		cpu_set_half_carry(&state, rand() % 2);
		cpu_set_subtract(&state, rand() % 2);
		cpu_set_carry(&state, test[2]);
		reg_t res = cpu_adc(&state, test[0], test[1]);
		if(res              == test[3] &&
		   cpu_zero(&state)       == test[4] &&
		   cpu_half_carry(&state) == test[5] &&
		   cpu_subtract(&state)   == test[6] &&
		   cpu_carry(&state)      == test[7])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%02x %d %d %d %d\n",  res, (int) cpu_zero(&state),
									 (int) cpu_half_carry(&state),
									 (int) cpu_subtract(&state),
									 (int) cpu_carry(&state));
		}
	}
}
