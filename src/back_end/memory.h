#pragma once
#include "types.h"

typedef struct memory memory_t;

memory_t *memory_init(const char *filename);
void      memory_delete(memory_t *mem);
reg_t     memory_load8(memory_t *mem, reg16_t addr);
void      memory_store8(memory_t *mem, reg16_t addr, reg_t data);
reg16_t   memory_load16(memory_t *mem, reg16_t addr);
void      memory_store16(memory_t *mem, reg16_t addr, reg16_t data);
void      memory_load_rom(memory_t *mem, const char *filename);
