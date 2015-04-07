#include "memory.h"
#include "logging.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "embedded.h"
#include "cpu.h"

#define WARN_INVALID_MEMORY 0

static reg_t read_IO_registers(memory_t *mem, reg16_t addr);
static void write_IO_registers(memory_t *mem, reg16_t addr, reg_t data);

memory_t *memory_init(cpu_state_t *state, const char *boot, const char *rom)
{
	memory_t *out = (memory_t*)calloc(1, sizeof(memory_t));

#if EMBEDDED
    out->bank_0 = malloc(rom_size);
    memcpy(out->bank_0, rom_array, rom_size);
    memcpy(out->boot, boot_array, 0x100);
#else
	FILE *fp = FOPEN(boot, "rb");
        common_fread(out->boot, 1, 0x100, fp);
    fclose(fp);

	fp = FOPEN(rom, "rb");
        fseek(fp, 0L, SEEK_END);
        out->to_read = (size_t) ftell(fp);
        fseek(fp, 0x0, SEEK_SET);
        out->bank_0       = (reg_t*)malloc(out->to_read);
        common_fread(out->bank_0, 1, out->to_read, fp);
	fclose(fp);
#endif

	out->state        = state;
	out->audio        = audio_init(state);
	out->bank_n       = out->bank_0;
	out->echo         = out->working_ram_0;
	out->current_bank = 1;
	out->boot_locked  = 0;
	out->cart_type    = (enum CART_TYPE)     out->bank_0[0x147];
	out->rom_size     = (enum ROM_SIZE)      out->bank_0[0x148];
	out->ram_size     = (enum SAVE_RAM_SIZE) out->bank_0[0x149];
	//Set all of the buttons to off.
	*(uint8_t*)&out->buttons = 0xff;
	*(uint8_t*)&out->dpad    = 0xff;
    *(uint8_t*)&out->lcdc    = 0x83;

    for(int i = 0; i < 40; i++)
    {
        out->oam_index_sort[i] = i;
    }

	switch(out->ram_size)
	{
        case SAVE_RAM_SIZE_0K:
            out->external_ram = NULL;
            break;
        case SAVE_RAM_SIZE_2K:
            out->external_ram = (uint8_t*) malloc(2 * 1024);
            break;
        case SAVE_RAM_SIZE_8K:
            out->external_ram = (uint8_t*) malloc(8 * 1024);
            break;
        case SAVE_RAM_SIZE_32K:
            out->external_ram = (uint8_t*) malloc(32 * 1024);
            break;
        default:
            log_error("Invalid Ram Size %d\n", out->ram_size);
	}
	return out;
}

void memory_delete(memory_t *mem)
{
	free(mem);
}

static void joypad_write(memory_t *mem, reg_t data)
{
	mem->buttons.enabled = 0;
	mem->dpad.enabled    = 0;
	*(uint8_t*)&mem->buttons |= data & 0x20;
	*(uint8_t*)&mem->dpad    |= data & 0x10;
}

static reg_t joypad_read(memory_t *mem)
{
	uint8_t out = 0;
	if(!mem->dpad.enabled)
	{
		out |= *(uint8_t*)&mem->dpad;
	}
	if(!mem->buttons.enabled)
	{
		out |= *(uint8_t*)&mem->buttons;
	}
	return out;
}

static void status(memory_t *mem, reg_t data)
{
    uint8_t mask = MASK(6) << 2;
	*(uint8_t*)&mem->stat = ((*(uint8_t*)&mem->stat) & ~ mask) | (data & mask);
}

struct OAM_data *oam_sort_data;
static int cmp_func(const void *a, const void *b)
{
    uint8_t xa = oam_sort_data[*(uint8_t*)a].x_pos;
    uint8_t xb = oam_sort_data[*(uint8_t*)b].x_pos;
    if(xa < xb) return -1;
    return 1;
}

static void sort_oam(memory_t *mem)
{
#if SORT_OAM
    oam_sort_data = mem->oam_data;
    qsort(mem->oam_index_sort, 40, sizeof(uint8_t), cmp_func);
#endif
}

static void dma(memory_t *mem, reg_t data)
{
	//Writing to the DMA register initiates a 0xa0 byte DMA transfer.

	//TODO:Make this occur over multiple clock cycles.
	//We need to restrict accesses to HRAM also.
	reg16_t source = data * 0x100;
	for(reg16_t i = 0; i < 0xa0; i++) //Transfer 40 * 32bit = 0xa0 bytes
	{
		mem->OAM[i] = memory_load8(mem, source + i);
	}
    sort_oam(mem);
}

static void hdma(memory_t *mem, reg_t data)
{
    //TODO: Implement (GBC Only?)
}


static reg_t read_IO_registers(memory_t *mem, reg16_t addr)
{
	if(0xff10 <= addr && addr < 0xff40)
	{
		return audio_load(mem->audio, addr);
	}
	switch(addr)
	{
		case 0xff00:
			return joypad_read(mem);
		case 0xff01:
			return mem->serial_data;
		case 0xff02:
			return mem->serial_control;
		//Timer Registers
		case 0xff04:
			return mem->div;
		case 0xff05:
			return mem->tima;
		case 0xff06:
			return mem->tma;
		case 0xff07:
			return *(uint8_t*)&mem->tac;
		case 0xff0f:
			return mem->IF;
		case 0xff40:
			return *(uint8_t*)&mem->lcdc;
		case 0xff41:
			return *(uint8_t*)&mem->stat;
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
        case 0xff4d: //Double speed mode (GBC Only)
            return 0;
		default:
			log_error("IO register not finished 0x%04x\n", addr);
	}
	return 0;
}


static void write_IO_registers(memory_t *mem, reg16_t addr, reg_t data)
{
	//TODO:IO Registers.
	if(0xff10 <= addr && addr < 0xff40)
	{
		audio_store(mem->audio, addr, data);
		return;
	}
	switch(addr)
	{
		case 0xff00: // Joypad.
			joypad_write(mem, data);
			break;
		case 0xff01: //TODO:Implement Serial transfer.
			mem->serial_data = data;
			break;
		case 0xff02:
			mem->serial_control = data;
            if(data == 0x81)
            {
                printf("%c", mem->serial_data);
                fflush(stdout);
            }
			break;
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
			*(uint8_t*)&mem->tac = data;
			break;
		case 0xff0f:
			mem->IF = data;
			break;
		//Video Registers
		case 0xff40:
			*(uint8_t*)&mem->lcdc = data;
			break;
		case 0xff41:
			status(mem, data);
			break;
		case 0xff42:
			mem->scy = data;
			break;
		case 0xff43:
			mem->scx = data;
			break;
		case 0xff44:
			Error("Read only.\n");
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
            {
				mem->boot_locked = 1;
#if INTRO_ONLY
    exit(0);
#endif
            }
			break;
        case 0xff55:
            hdma(mem, data);
            break;
        case 0xff70:
            break;//TODO: SVBK (CGB Only)
		case 0xff7f:
			//This seems to be accessed by accident when cleaning memory.
			break;
		default:
			log_verbose("IO register not finished 0x%04x\n", addr);
	}
}

#define X(min, max) (addr <= max)
//#define X(min, max) ((addr >= min) && (addr <= max))
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
		return mem->bank_n[mem->current_bank * 0x4000 + addr - 0x4000];
	}
	else if(X(0x8000,0x9fff))
	{
		return mem->video_ram[addr - 0x8000];
	}
	else if(X(0xa000,0xbfff))
	{
        if(mem->external_ram && mem->ram_enabled)
            return mem->external_ram[mem->ram_bank * 0x8000 + addr - 0xa000];
#if WARN_INVALID_MEMORY
        else
            log_warning("Accessing invalid memory.\n");
#endif
        return 0;
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
	else// if(X(0xffff,0xffff))
	{
		return mem->IE;
	}
}

void memory_store8(memory_t *mem, reg16_t addr, reg_t data)
{
	if(X(0x0000, 0x1fff))//RAM Enable
	{
        if((data & MASK(4)) == 0x0A)
        {
            mem->ram_enabled = 1;
        }
        else
        {
            mem->ram_enabled = 0;
        }
	}
	else if(X(0x2000, 0x3fff))//Select the lower 5 bits of the rom bank.
	{
        if(IS_MBC1(mem))
        {
            data = data & MASK(5);
            if(data == 0x0)
                data = 0x1;
            mem->current_bank = (mem->current_bank & 0x60) | data;
            if(mem->current_bank == 0) mem->current_bank = 1;
        }
        if(IS_MBC3(mem))
        {
            mem->current_bank = data & MASK(7);
            if(mem->current_bank == 0) mem->current_bank = 1;
        }
	}
	else if(X(0x4000, 0x5fff))//Select the upper 2 bits of the ROM or RAM bank.
	{
        if(IS_MBC1(mem))
        {
            if(mem->rom_ram_mode)
            {
                mem->ram_bank = data & MASK(2);
            }
            else
            {
                data = (data & MASK(2)) << 5;
                mem->current_bank = data | (mem->current_bank & MASK(5));
                if(mem->current_bank == 0) mem->current_bank = 1;
            }
        }
        if(IS_MBC3(mem))
        {
            if(data <= 0x3)
            {
                mem->ram_bank = data;
            }
            else if(data <= 0x8)
            {
                log_error("Not implemented.\n");
            }
        }
	}
	else if(X(0x6000, 0x7fff))//ROM/RAM mode select.
	{
		mem->rom_ram_mode = data & 0x1;
	}
	else if(X(0x8000,0x9fff))
	{
		mem->video_ram[addr - 0x8000] = data;
	}
	else if(X(0xa000,0xbfff))
	{
        if(mem->external_ram && mem->ram_enabled)
            mem->external_ram[mem->ram_bank * 0x8000 + addr - 0xa000] = data;
#if WARN_INVALID_MEMORY
        else
            log_warning("Writing to invalid memory.\n");
#endif
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
	else// if(X(0xffff,0xffff))
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

const uint8_t *memory_get_tile_data(memory_t *memory, int tx, int ty, int offset, int map)
{
	//Tile map is located at address 0x9800 or 0x9c00
	uint8_t tile = memory_get_tile_index(memory, tx, ty, map);
	//Tils data is located at addresses
	// 0x8800 -> 97FF or
	// 0x8000 -> 8FFF
	uint8_t *tile_data;
	if(memory->lcdc.tile_data_select)
	{
		tile_data = &memory->video_ram[tile * 16];
	}
	else
	{
		/* The tiles are in the range -128 to 127 so we cast the
		   binary representation of the tile to twos-complement */
		tile_data = &memory->video_ram[0x1000 +  *(int8_t*)&tile * 16];
	}
	return tile_data + 2 * offset;
}

uint8_t memory_get_tile_index(memory_t *memory, int tx, int ty, int map)
{
	uint8_t *video_ram = memory->video_ram;
	int tile_num = ty * 32 + tx;
	return video_ram[(map ? 0x1c00 : 0x1800) + tile_num];
}

audio_t *memory_get_audio(memory_t *memory)
{
    return memory->audio;
}

#define X(elem) fwrite(&memory->elem, 1, sizeof(memory->elem), fp)
#define Y(elem) fwrite(memory->elem, 1, sizeof(memory->elem), fp)
void  memory_save_state(memory_t *memory, FILE *fp)
{
	Y(video_ram);
	Y(working_ram_0);
	Y(working_ram_1);
	Y(OAM);
	Y(io_registers);
	Y(stack);
	X(boot_locked);
	Y(boot);
	X(current_bank);
	X(ram_size);
	X(cart_type);
	X(rom_size);
	X(ram_enabled);
	X(rom_ram_mode);
	X(lcdc);
	X(buttons);
	X(dpad);
	X(stat);
	X(scy);
	X(scx);
	X(ly);
	X(lyc);
	X(dma);
	X(bgp);
	X(obp0);
	X(obp1);
	X(wy);
	X(wx);
	X(IF);
	X(IE);
	X(IME);
	X(serial_data);
	X(serial_control);
	X(div);
	X(tima);
	X(tma);
	X(tac);
	X(to_read);
	fwrite(memory->bank_0, sizeof(*memory->bank_0) * memory->to_read, 1, fp);
}
#undef X
#undef Y

#define X(elem) fread(&memory->elem, 1, sizeof(memory->elem), fp)
#define Y(elem) fread(memory->elem, 1, sizeof(memory->elem), fp)
memory_t *memory_load_state(FILE *fp)
{
	memory_t *memory = (memory_t*) malloc(sizeof(memory_t));
	Y(video_ram);
	Y(working_ram_0);
	Y(working_ram_1);
	Y(OAM);
	Y(io_registers);
	Y(stack);
	X(boot_locked);
	Y(boot);
	X(current_bank);
	X(ram_size);
	X(cart_type);
	X(rom_size);
	X(ram_enabled);
	X(rom_ram_mode);
	X(lcdc);
	X(buttons);
	X(dpad);
	X(stat);
	X(scy);
	X(scx);
	X(ly);
	X(lyc);
	X(dma);
	X(bgp);
	X(obp0);
	X(obp1);
	X(wy);
	X(wx);
	X(IF);
	X(IE);
	X(IME);
	X(serial_data);
	X(serial_control);
	X(div);
	X(tima);
	X(tma);
	X(tac);
	X(to_read);
	memory->bank_0 = (uint8_t*) malloc(memory->to_read);
	fread(memory->bank_0, memory->to_read, 1, fp);
	memory->bank_n       = memory->bank_0 + memory->current_bank * 0x4000;
	memory->external_ram = (uint8_t*) malloc(10 * 1024);
	memory->echo         = memory->working_ram_0;
	return memory;
}
#undef X
#undef Y
