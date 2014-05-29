#include <stdio.h>
#include <string.h>

#include "rom.h"
#include "cpu.h"

int main(int argc, char **argv)
{
	struct cpu_state state;
	memset(&state, 0x00, sizeof(struct cpu_state));
	for(int i = 0; i < 0xff; i++)
	{
		struct opcode code = op_table[i];
		code.op(&state, code.arg0, code.i0, code.arg1, code.i1);
	}
	return 0;
}
