#include "testing.h"

void daa_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0x3a, 0xc6, 0x00, 1, 1, 0, 1},
		{0x3c, 0xff, 0x3b, 0, 1, 0, 1},
		{0x3c, 0x12, 0x4e, 0, 0, 0, 0},
	};
	for(uint32_t i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		cpu_set_half_carry(&state, rand() % 2);
		cpu_set_subtract(&state, rand() % 2);
		cpu_set_carry(&state, rand() % 2);
		reg_t res = cpu_add8(&state, test[0], test[1]);
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
			printf("0x%02x %d %d %d %d\n",  res, (int) cpu_zero(&state),
									 (int) cpu_half_carry(&state),
									 (int) cpu_subtract(&state),
									 (int) cpu_carry(&state));
		}
	}
}
