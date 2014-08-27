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


int testing(int argc, char **argv)
{
	//flag_test();
	//temp();
	//adc_test();
	printf("cmp.\n");
	cmp_test();
	printf("sub.\n");
	sub_test();
	printf("swap.\n");
	swap_test();
	printf("jr.\n");
	jr_test();
	printf("call.\n");
	call_test();
	printf("bit.\n");
	bit_test();
	printf("dec.\n");
	dec_test();
	printf("inc.\n");
	inc_test();
	printf("ret.\n");
	ret_test();
	printf("pop.\n");
	pop_test();
	return 0;
}
