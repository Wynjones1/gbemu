#include "testing.h"

static void inc8_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][6] =
	{
		{0x00, 0x01, 0, 0, 0, 0},
		{0x0f, 0x10, 0, 1, 0, 0},
		{0xff, 0x00, 1, 1, 0, 0},
		{0xf0, 0xf1, 0, 0, 0, 0},
	};
	for(uint32_t i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		uint8_t res = cpu_inc8(&state, test[0]);
		cpu_set_carry(&state, test[5]);
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
			printf("%d %d %d %d\n",  (int) cpu_zero(&state),
									 (int) cpu_half_carry(&state),
									 (int) cpu_subtract(&state),
									 (int) cpu_carry(&state));
		}
	}
}

static void inc16_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][2] = 
	{
		{0x0000, 0x0001},
		{0x00ff, 0x0100},
		{0xffff, 0x0000},
		{0x0fff, 0x1000},
	};
	for(uint32_t i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		REG_INPUT reg = REG16_BC;
		state.bc = test[0];
		cpu_inc16(&state, reg);
		if(state.bc == test[1])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
		}
	}
}

void inc_test(void)
{
	inc8_test();
	inc16_test();
}
