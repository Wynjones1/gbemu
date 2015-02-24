#include "testing.h"

void push_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][3] =
	{
		{0xfffe, 0x1000, 0xfffc},
	};
	for(uint32_t i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.sp = test[0];
		cpu_push(&state, test[1]);
		if(state.sp == test[2] && cpu_load16(&state, test[2]) == test[1])
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
