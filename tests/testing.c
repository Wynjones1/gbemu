#include "testing.h"
#include <stdint.h>
#include <assert.h>

#define TEST_LIST \
X(adc)            \
X(flag)           \
X(cmp)            \
X(sub)            \
X(swap)           \
X(jr)             \
X(call)           \
X(bit)            \
X(dec)            \
X(inc)            \
X(ret)            \
X(pop)            \
X(or)             \
X(xor)            \
X(and)            \
X(rra)            \
X(rl)             \
X(rla)            \
X(rr)             \
X(srl)            \
X(add)            \

#define X(testname) void testname ## _test(void);
TEST_LIST
#undef X


int main(int argc, char **argv)
{
	#define X(name) printf(#name ".\n"); name ## _test();
		TEST_LIST
	#undef X
	return 0;
}

