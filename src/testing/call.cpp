#include "testing.h"

void call_test(void)
{
	cpu_state_t state;
	int *test;
	int tests[][7] =
	{
		//PC, SP, ADDR, (SP - 1), (SP - 2), new SP
		{0x8000, 0xfffe, 0x1234, 0x8003, 0xfffc},
	};
	for(unsigned int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = tests[i];
		state.pc = test[0] + 3;
		state.sp = test[1];
		cpu_call(&state, test[2]);
		if(state.pc == test[2] &&
		   state.sp == test[4] &&
		   cpu_load8(&state, test[1] - 1) == (test[3] >> 8) &&
		   cpu_load8(&state, test[1] - 2) == (test[3] & 0xff))
		{
			printf("Tests passed.\n");
		}
		else
		{
			printf("Tests failed.\n");
			printf("0x%04x 0x%04x\n", state.pc, state.sp);
			printf("0x%02x = 0x%02x\n", cpu_load8(&state, test[1] - 1), test[3] >> 8);
			printf("0x%02x = 0x%02x\n", cpu_load8(&state, test[1] - 2), test[3] & 0xff);
		}
	}
}
