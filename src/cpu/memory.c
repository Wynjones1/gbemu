#include "memory.h"

typedef struct memory
{
	reg_t data[65536];
}memory_t;

void    memory_init(memory_t *mem)
{}

reg_t   memory_load8(memory_t *mem, reg16_t addr)
{}

reg16_t memory_load16(memory_t *mem, reg16_t addr)
{}

void    memory_store8(memory_t *mem, reg16_t addr, reg_t data)
{}

void    memory_store16(memory_t *mem, reg16_t addr, reg16_t data)
{}
