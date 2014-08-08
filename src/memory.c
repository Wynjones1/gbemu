#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include "common.h"

static reg_t read_IO_registers(memory_t *mem, reg16_t addr);
static void write_IO_registers(memory_t *mem, reg16_t addr, reg_t data);


memory_t *memory_init(const char *boot, const char *rom)
{
	memory_t *out = malloc(sizeof(memory_t));
	//TODO: Add safe read functions.
	FILE *fp = fopen(boot, "r");
	fread(out->boot, 1, 0x100, fp);
	fclose(fp);

	fp = fopen(rom, "r");
	fseek(fp, 0L, SEEK_END);
	int to_read = ftell(fp);
	fseek(fp, 0x0, SEEK_SET);
	out->bank_0       = malloc(to_read);
	out->bank_n       = out->bank_0 + 0x4000;
	out->echo         = out->working_ram_0;
	out->current_bank = 1;
	out->io_registers[0x44] = 0x90;
	out->cart_type = out->bank_0[0x147];
	fread(out->bank_0, 1, to_read, fp);
	fclose(fp);
	for(int i = 0; i < 0x2000; i++)
		out->video_ram[i] = rand();

	return out;
}

void memory_delete(memory_t *mem)
{
	free(mem);
}

static void lcdc(memory_t *mem, reg_t data)
{
	mem->lcdc = data;
	Warning("Not Implemented fully.\n");
}
static void stat(memory_t *mem, reg_t data)
{
	Error("Not Implemented.\n");
}
static void scy(memory_t *mem, reg_t data)
{
	mem->scy = data;
}
static void scx(memory_t *mem, reg_t data)
{
	mem->scx = data;
}
static void ly(memory_t *mem, reg_t data)
{
	Error("Not Implemented.\n");
}
static void lyc(memory_t *mem, reg_t data)
{
	Error("Not Implemented.\n");
}
static void dma(memory_t *mem, reg_t data)
{
	Error("Not Implemented.\n");
}
static void bgp(memory_t *mem, reg_t data)
{
	mem->bgp = data;
	Warning("Not Implemented fully.\n");
}
static void obp0(memory_t *mem, reg_t data)
{
	Error("Not Implemented.\n");
}
static void obp1(memory_t *mem, reg_t data)
{
	Error("Not Implemented.\n");
}
static void wy(memory_t *mem, reg_t data)
{
	Error("Not Implemented.\n");
}
static void wx(memory_t *mem, reg_t data)
{
	Error("Not Implemented.\n");
}

#define X(min, max) addr >= min && addr <= max
reg_t memory_load8(memory_t *mem, reg16_t addr)
{
	if(addr == 0xff44) return 0x90;//TODO: Fix
	if(X(0x0000,0x3fff))
	{
		if(addr < 0x100 && !mem->boot_locked)
		{
			return mem->boot[addr];
		}
		return mem->bank_0[addr];
	}
	else if(X(0x4000,0x7fff))
	{
		return mem->bank_n[addr - 0x4000];
	}
	else if(X(0x8000,0x9fff))
	{
		return mem->video_ram[addr - 0x8000];
	}
	else if(X(0xa000,0xbfff))
	{
		return mem->external_ram[addr - 0xa000];
	}
	else if(X(0xc000,0xcfff))
	{
		return mem->working_ram_0[addr - 0xc000];
	}
	else if(X(0xd000,0xdfff))
	{
		return mem->working_ram_1[addr - 0xd000];
	}
	else if(X(0xe000,0xfdff))
	{
		return mem->echo[addr - 0xe000];
	}
	else if(X(0xfe00,0xfe9f))
	{
		return mem->OAM[addr - 0xfe00];
	}
	else if(X(0xfea0,0xfeff))
	{
		//Unused.
	}
	else if(X(0xff00,0xff7f))
	{
		return read_IO_registers(mem, addr);
	}
	else if(X(0xff80,0xfffe))
	{
		return mem->stack[addr - 0xff80];
	}
	else if(X(0xffff,0xffff))
	{
		return mem->interrupt_enable;
	}
	return 0;
}

static reg_t read_IO_registers(memory_t *mem, reg16_t addr)
{
	switch(addr)
	{
		case 0xff40:
			return mem->lcdc;
		case 0xff41:
			return mem->stat;
		case 0xff42:
			return mem->scx;
		case 0xff43:
			return mem->scy;
		case 0xff44:
			return mem->ly;
		case 0xff45:
			return mem->lyc;
		case 0xff46:
			return mem->dma;
		case 0xff47:
			return mem->bgp;
		case 0xff48:
			return mem->obp0;
		case 0xff49:
			return mem->obp1;
		case 0xff4a:
			return mem->wy;
		case 0xff4b:
			return mem->wx;
		default:
			Error("IO Registers not done %04x\n", addr);
	}
	return 0;
}


static void write_IO_registers(memory_t *mem, reg16_t addr, reg_t data)
{
	//TODO:IO Registers.
	if(X(0xff10, 0xff3f))
	{
		Warning("Sound function not available (addr : 0x%04x)\n", addr);
		return;
	}
	switch(addr)
	{
		case 0xff0f:
			Error("Interrupt Flag not implemented.\n");
			break;
		//Video Registers
		case 0xff40:
			lcdc(mem, data);
			break;
		case 0xff41:
			stat(mem, data);
			break;
		case 0xff42:
			scx(mem, data);
			break;
		case 0xff43:
			scy(mem, data);
			break;
		case 0xff44:
			ly(mem, data);
			break;
		case 0xff45:
			lyc(mem, data);
			break;
		case 0xff46:
			dma(mem, data);
			break;
		case 0xff47:
			bgp(mem, data);
			break;
		case 0xff48:
			obp0(mem, data);
			break;
		case 0xff49:
			obp1(mem, data);
			break;
		case 0xff4a:
			wy(mem, data);
			break;
		case 0xff4b:
			wx(mem, data);
			break;
		case 0xff50:
			if(data)
				mem->boot_locked = 1;
			break;
		default:
			Error("IO Registers not done %04x\n", addr);
	}
}

void memory_store8(memory_t *mem, reg16_t addr, reg_t data)
{
	if(X(0x0000,0x3fff))
	{
		Error("Cannot write to ROM addr: 0x%x\n", addr);
	//	mem->bank_0[addr] = data;
	}
	else if(X(0x4000,0x7fff))
	{
		Error("Cannot write to ROM addr: 0x%x\n", addr);
	//	mem->bank_n[addr - 0x4000] = data;
	}
	else if(X(0x8000,0x9fff))
	{
		if(X(0x9800, 0x9bff))
		{
		//	printf("Writing to background tile map.\n");
		}
		mem->video_ram[addr - 0x8000] = data;
	}
	else if(X(0xa000,0xbfff))
	{
		mem->external_ram[addr - 0xa000] = data;
	}
	else if(X(0xc000,0xcfff))
	{
		mem->working_ram_0[addr - 0xc000] = data;
	}
	else if(X(0xd000,0xdfff))
	{
		mem->working_ram_1[addr - 0xd000] = data;
	}
	else if(X(0xe000,0xfdff))
	{
		mem->echo[addr - 0xe000] = data;
	}
	else if(X(0xfe00,0xfe9f))
	{
		mem->OAM[addr - 0xfe00] = data;
	}
	else if(X(0xfea0,0xfeff))
	{
		//Unused.
	}
	else if(X(0xff00,0xff7f))
	{
		write_IO_registers(mem, addr, data);
	}
	else if(X(0xff80,0xfffe))
	{
		mem->stack[addr - 0xff80] = data;
	}
	else if(X(0xffff,0xffff))
	{
		mem->interrupt_enable = data;
	}
}
#undef X

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
{}
