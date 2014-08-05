#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct memory
{
	reg_t interrupt[0x100];               // 0x0000 -> 0x0100
	reg_t rom[0x150 - 0x100];             // 0x0100 -> 0x0150
	reg_t data[0x8000 - 0x150];           // 0x0150 -> 0x8000
	reg_t *bank0;                         // 0x8000 -> 0x9800
	reg_t display_data1[0x9C00 - 0x9800]; // 0x9800 -> 0x9C00
	reg_t display_data2[0x9C00 - 0x9800]; // 0x9C00 -> 0xA000
	reg_t *external_working_ram;          // 0xA000 -> 0xC000
	reg_t *unit_working_ram;              // 0xC000 -> 0xE000
	reg_t unusable0[0xFE00 - 0xE000];     // 0xE000 -> 0xFDFF
	reg_t OAM[0xFEA0 - 0xFE00];           // 0xFE00 -> 0xFEA0
	reg_t unusable2[0xFF00 - 0xFEA0];     // 0xFEA0 -> 0xFF00
	reg_t *port_registers;                // 0xFF00 -> 0xFF80
	reg_t stack[0xFFFE - 0xFF80];         // 0xFF80 -> 0xFFFE
	reg_t flags;                          // 0xFFFE -> 0xFFFF

	//Boot Rom.
	reg_t boot[0x100];
}memory_t;

memory_t *memory_init(const char *filename)
{
	memory_t *out = malloc(sizeof(memory_t));
	//TODO: Add safe read functions.
	FILE *fp = fopen(filename, "r");
	fread(out->boot, 1, 0x100, fp);
	fclose(fp);
	return out;
}

void memory_delete(memory_t *mem)
{
	free(mem);
}

reg_t   memory_load8(memory_t *mem, reg16_t addr)
{
	//TODO:Implement
	return mem->boot[addr];
}

reg16_t memory_load16(memory_t *mem, reg16_t addr)
{
	//TODO:Implement
	return *(reg16_t*)&mem->boot[addr];
}

void    memory_store8(memory_t *mem, reg16_t addr, reg_t data)
{
	//TODO:Implement
	mem->boot[addr] = data;
}

void    memory_store16(memory_t *mem, reg16_t addr, reg16_t data)
{
	//TODO:Implement
	*(reg16_t*)&mem->boot[addr] = data;
}
