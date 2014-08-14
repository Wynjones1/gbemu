#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

static reg_t read_IO_registers(memory_t *mem, reg16_t addr);
static void write_IO_registers(memory_t *mem, reg16_t addr, reg_t data);

#define DEBUG_DMA 0


memory_t *memory_init(const char *boot, const char *rom)
{
	memory_t *out = calloc(1, sizeof(memory_t));
	//TODO: Add safe read functions.
	FILE *fp = fopen(boot, "r");
	fread(out->boot, 1, 0x100, fp);
	fclose(fp);

	fp = fopen(rom, "r");
	fseek(fp, 0L, SEEK_END);
	int to_read = ftell(fp);
	fseek(fp, 0x0, SEEK_SET);
	out->bank_0       = malloc(to_read);
	fread(out->bank_0, 1, to_read, fp);

	out->bank_n       = out->bank_0 + 0x4000;
	out->echo         = out->working_ram_0;
	out->current_bank = 1;
	out->io_registers[0x44] = 0x90;
	out->boot_locked = 0;
	fclose(fp);
	out->cart_type = out->bank_0[0x147];
	out->rom_size  = out->bank_0[0x148];
	out->ram_size  = out->bank_0[0x149];

	switch(out->ram_size)
	{
		case 0:
			break;
		case 1:
			out->external_ram = malloc(2 * 1024);
			break;
		case 2:
			out->external_ram = malloc(4 * 1024);
			break;
		case 3:
			out->external_ram = malloc(32 * 1024);
			break;
	}
	out->external_ram = malloc(10 * 1024);
	//Error("Ram Size %d\n", out->ram_size);
	return out;
}

void memory_delete(memory_t *mem)
{
	free(mem);
}

static void stat(memory_t *mem, reg_t data)
{
	mem->stat = data;
}

static void dma(memory_t *mem, reg_t data)
{
#if DEBUG_DMA
	static FILE *fp;
	if(!fp) fp = fopen("dma_log.txt", "w");
#endif
	//Writing to the DMA register initiates a 0x100 byte DMA transfer.

	//TODO:Make this occur over multiple clock cycles.
	//We need to restrict accesses to HRAM also.
	reg16_t source = data * 0x100;
	for(reg16_t i = 0; i < 0x100; i++)
	{
		mem->OAM[i] = memory_load8(mem, source + i);
#if DEBUG_DMA
		static int count;
		fprintf(fp, "%02x ", mem->OAM[i]);
		count++;
		if(count == 0x10)
		{
			fprintf(fp,"\n");
			count = 0;
		}
#endif
	}
#if DEBUG_DMA
	fprintf(fp, "\n");
#endif
}

#define X(min, max) ((addr >= min) && (addr <= max))
reg_t memory_load8(memory_t *mem, reg16_t addr)
{
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
		return mem->bank_0[(mem->current_bank - 1) * 0x4000 + addr];
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
		return 0xff;
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
		return mem->IE;
	}
	return 0;
}

static reg_t read_IO_registers(memory_t *mem, reg16_t addr)
{
	switch(addr)
	{
		//Timer Registers
		case 0xff04:
			return mem->div;
			break;
		case 0xff05:
			return mem->tima;
			break;
		case 0xff06:
			return mem->tma;
			break;
		case 0xff07:
			return mem->tac;
			break;
		case 0xff0f:
			return mem->IF;
		case 0xff40:
			return *(uint8_t*)&mem->lcdc;
		case 0xff41:
			return mem->stat;
		case 0xff42:
			return mem->scy;
		case 0xff43:
			return mem->scx;
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
		case 0xff00:
			return mem->joypad;
		case 0xff01:
			return mem->serial_data;
		case 0xff02:
			return mem->serial_control;
		default:
			Error("IO register not finished 0x%04x\n", addr);
	}
	return 0;
}


static void write_IO_registers(memory_t *mem, reg16_t addr, reg_t data)
{
	//TODO:IO Registers.
	if(X(0xff10, 0xff3f))
	{
#if 0
		Warning("Sound function not available (addr : 0x%04x)\n", addr);
#endif
		return;
	}
	switch(addr)
	{
		//Timer Registers
		case 0xff04:
			mem->div = 0;
			break;
		case 0xff05:
			mem->tima = data;
			break;
		case 0xff06:
			mem->tma = data;
			break;
		case 0xff07:
			mem->tac = data;
			break;
		case 0xff0f:
			mem->IF = data;
			break;
		//Video Registers
		case 0xff40:
			*(uint8_t*)&mem->lcdc = data;
			break;
		case 0xff41:
			stat(mem, data);
			break;
		case 0xff42:
			mem->scy = data;
			break;
		case 0xff43:
			mem->scx = data;
			break;
		case 0xff44:
			mem->ly = data;
			break;
		case 0xff45:
			mem->lyc = data;
			break;
		case 0xff46:
			dma(mem, data);
			break;
		case 0xff47:
			mem->bgp = data;
			break;
		case 0xff48:
			mem->obp0 = data;
			break;
		case 0xff49:
			mem->obp1 = data;
			break;
		case 0xff4a:
			mem->wy = data;
			break;
		case 0xff4b:
			mem->wx = data;
			break;
		case 0xff50:
			if(data)
				mem->boot_locked = 1;
			break;
		case 0xff7f:
			//This seems to be accessed by accident when cleaning memory.
			break;
		case 0xff00: // Joypad.
			mem->joypad = data;
			break;
		case 0xff01: //TODO:Implement Serial transfer.
			mem->serial_data = data;
			break;
		case 0xff02:
			mem->serial_control = data;
			break;
		default:
			Error("IO Registers not done %04x\n", addr);
	}
}

void memory_store8(memory_t *mem, reg16_t addr, reg_t data)
{
	if(X(0x0000, 0x1fff))//RAM Enable
	{
		mem->ram_enabled = data;
	}
	else if(X(0x2000, 0x3fff))//Select the lower 5 bits of the rom bank.
	{
		mem->lsb = data & 0x1f;
		if(mem->current_bank == 0x00 ||
			mem->current_bank == 0x20 ||
			mem->current_bank == 0x40 ||
			mem->current_bank == 0x60)
		{
			mem->current_bank++;
		}
	}
	else if(X(0x4000, 0x5fff))//Select the upper 2 bits of the ROM bank or the RAM bank.
	{
		mem->msb = data & 0x3;
		if(mem->current_bank == 0x00 ||
			mem->current_bank == 0x20 ||
			mem->current_bank == 0x40 ||
			mem->current_bank == 0x60)
		{
			mem->current_bank++;
		}
	}
	else if(X(0x6000, 0x7fff))//ROM/RAM mode select.
	{
		data &= 0x1;
		mem->rom_ram_mode = data;
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
		write_IO_registers(mem, addr, data);
	}
	else if(X(0xff80,0xfffe))
	{
		mem->stack[addr - 0xff80] = data;
	}
	else if(X(0xffff,0xffff))
	{
		mem->IE = data;
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
