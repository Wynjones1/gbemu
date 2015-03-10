#include "testing.h"

void pop_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		{0xfffc, 0x5f, 0xfffd, 0x3c, 0xfffe}
	};
	for(uint32_t i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.sp = test[0];
		cpu_store8(&state, test[0], test[1]);
		cpu_store8(&state, test[2], test[3]);
		REG_INPUT input = REG16_BC;
		cpu_pop(&state, input);
		if(state.sp == test[4] && state.b == test[3] && state.c == test[1])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%04x 0x%04x\n", state.bc, state.sp);
		}
	}
}
