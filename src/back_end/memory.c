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
	int boot_locked;
	reg_t boot[0x100];

	FILE *temp;

}memory_t;

memory_t *memory_init(const char *filename)
{
	memory_t *out = malloc(sizeof(memory_t));
	//TODO: Add safe read functions.
	FILE *fp = fopen(filename, "r");
	fread(out->boot, 1, 0x100, fp);
	fclose(fp);

	fp = fopen("/home/stuart/mario.gb", "r");
	fseek(fp, 0x100, SEEK_SET);
	fread(out->rom, 1, 0x50, fp);
	fclose(fp);

	out->boot_locked = 0;
	return out;
}

void memory_delete(memory_t *mem)
{
	free(mem);
}

reg_t   memory_load8(memory_t *mem, reg16_t addr)
{
	//TODO:Implement
	if(addr == 0xff44)
	{
		return 0x90;
	}
	else if(addr < 0x100)
	{
		return mem->boot[addr];
	}
	else
	{
		fseek(mem->temp, addr, SEEK_SET);
		return getc(mem->temp);
	}
}

void memory_store8(memory_t *mem, reg16_t addr, reg_t data)
{
	//TODO:Implement
	if(addr == 0xff50)
	{
	
	}
	else
	{
		mem->boot[addr] = data;
	}
}

reg16_t memory_load16(memory_t *mem, reg16_t addr)
{
	reg16_t out = memory_load8(mem, addr);
	out |= memory_load8(mem, addr + 1) << 8;
	return out;
}

void memory_store16(memory_t *mem, reg16_t addr, reg16_t data)
{
	memory_store8(mem, addr,     data & 0xff);
	memory_store8(mem, addr + 1, data >> 8);
}

void memory_load_rom(memory_t *mem, const char *filename)
{
	mem->temp = fopen(filename, "r");
}
