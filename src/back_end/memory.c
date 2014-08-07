#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include "common.h"

typedef struct memory
{
	reg_t *bank_0;               // 0x0000 -> 0x3fff
	reg_t *bank_n;               // 0x4000 -> 0x7fff
	reg_t  video_ram[0x2000];    // 0x8000 -> 0x9fff
	reg_t *external_ram;         // 0xa000 -> 0xbfff
	reg_t working_ram_0[0x1000]; // 0xc000 -> 0xcfff
	reg_t working_ram_1[0x1000]; // 0xd000 -> 0xdfff //CGB: Should be switchable.
	reg_t *echo       ;          // 0xe000 -> 0xfdff
	reg_t OAM[0x1e00];           // 0xfe00 -> 0xfe9f
	//unused                     // 0xfea0 -> 0xfeff
	reg_t io_registers[0x80];    // 0xff00 -> 0xff7f
	reg_t stack[0x60];           // 0xff80 -> 0xfffe
	reg_t interrupt_enable;      // 0xffff

	//Boot Rom.
	int boot_locked;
	reg_t boot[0x100];
	//Banking information;
	int current_bank;
	int cart_type;
	//IO Status registers.
	int lcdc;
	int stat;
	int scy;
	int scx;
	int ly;
	int lyc;
	int dma;
	int bgp;
	int obp0;
	int obp1;
	int wy;
	int wx;

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

	return out;
}

void memory_delete(memory_t *mem)
{
	free(mem);
}

static void lcdc(memory_t *mem, reg_t data)
{}
static void stat(memory_t *mem, reg_t data)
{}
static void scy(memory_t *mem, reg_t data)
{}
static void scx(memory_t *mem, reg_t data)
{}
static void ly(memory_t *mem, reg_t data)
{}
static void lyc(memory_t *mem, reg_t data)
{}
static void dma(memory_t *mem, reg_t data)
{}
static void bgp(memory_t *mem, reg_t data)
{}
static void obp0(memory_t *mem, reg_t data)
{}
static void obp1(memory_t *mem, reg_t data)
{}
static void wy(memory_t *mem, reg_t data)
{}
static void wx(memory_t *mem, reg_t data)
{}

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
		//TODO:IO Registers.
		if(X(0xff10, 0xff3f))
		{
			Warning("Sound function not available (addr : 0x%04x)\n", addr);
			return;
		}
		switch(addr)
		{
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
{
	mem->temp = fopen(filename, "r");
}
