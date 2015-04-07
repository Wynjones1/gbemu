#include "testing.h"
#include <stdint.h>
#include <assert.h>

void adc_test(void);
void flag_test(void);
void cmp_test(void);
void sub_test(void);
void swap_test(void);
void jr_test(void);
void call_test(void);
void bit_test(void);
void dec_test(void);
void inc_test(void);
void ret_test(void);
void pop_test(void);
void or_test(void);
void xor_test(void);
void and_test(void);
void rra_test(void);
void rl_test(void);
void rla_test(void);
void rr_test(void);
void srl_test(void);
void add_test(void);


#define X(name) printf(#name ".\n"); name ## _test();
int testing(int argc, char **argv)
{
	//flag_test();
	//temp();
	X(cmp);
	X(sub);
	X(swap);
	X(jr);
	X(call);
	X(bit);
	X(dec);
	X(inc);
	X(ret);
	X(pop);
	X(or);
	X(xor);
	X(rra);
	X(adc);
	X(rl);
	X(rla);
	X(rr);
	X(srl);
	X(add);
	X(and);
	return 0;
}
