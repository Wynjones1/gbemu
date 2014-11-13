#include "testing.h"

void ret_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		//PC, SP, ADDR, (SP - 2), new SP
		{0x9000, 0xfff0, 0x8003, 0xfff2},
	};
	for(unsigned int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.pc = test[0] + 3;
		state.sp = test[1];
		cpu_store16(&state, state.sp, test[2]);
		cpu_ret(&state);
		if(state.pc == test[2] && state.sp == test[3])
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%04x 0x%04x\n", state.pc, state.sp);
		}
	}
}
