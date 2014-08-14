#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include "cpu.h"
#include "opcodes.h"
#include "ppm.h"

#include <SDL2/SDL.h>

#if 0
#define CPU_ERROR(state, arg0, i0, arg1, i1)\
	fprintf(stderr, "%s %s %d %s %d\n", __func__, ARG_TYPE_s[arg0], i0.r8, ARG_TYPE_s[arg1], i1.r16);\
	exit(-1)
#else
	#define CPU_ERROR(state, arg0, i0, arg1, i1)
#endif

#define ERR()\
	fprintf(stderr, "%s %s %d %s %d %d\n", __func__, ARG_TYPE_s[arg0], i0.r8, ARG_TYPE_s[arg1], i1.r16, __LINE__);\
	exit(-1)

cpu_state_t *cpu_init(const char *boot_rom_filename, const char *rom)
{
	cpu_state_t *out = calloc(1, sizeof(cpu_state_t));
	out->memory      = memory_init(boot_rom_filename, rom);
	out->display     = display_init(out->memory);
	return out;
}

void cpu_delete(cpu_state_t *state)
{
	memory_delete(state->memory);
	free(state);
}

/* CPU Helper functions */
reg_t   cpu_load_reg8(struct cpu_state *state, REG_INPUT reg)
{
	return state->registers[reg.r8];
}

reg16_t cpu_load_reg16(struct cpu_state *state, REG_INPUT reg)
{
	return state->registers16[reg.r16];
}

void cpu_store_reg8(struct cpu_state *state, REG_INPUT reg, reg_t data)
{
	state->registers[reg.r8] = data;
}

void cpu_store_reg16(struct cpu_state *state, REG_INPUT reg, reg16_t data)
{
	state->registers16[reg.r16] = data;
}

int cpu_carry(struct cpu_state *state)
{
	return state->carry;
}

int cpu_zero(struct cpu_state *state)
{
	return state->zero;
}

void cpu_load_rom(struct cpu_state *state, const char *filename)
{
	memory_load_rom(state->memory, filename);
}

const char *reg_strings[] =
{
	"F",
	"A", 
	"C",
	"B", 
	"E",
	"D", 
	"L",
	"H"
};

const char *reg16_strings[] =
{
	"AF",
	"BC",
	"DE",
	"HL",
	"SP",
};

#define X(n) case ARG_TYPE_ ## n:sprintf(buf, #n);break
void print_arg(char *buf, struct cpu_state *state, struct opcode *op,enum ARG_TYPE arg, REG_INPUT r)
{
	uint8_t rel;
	switch(arg)
	{
		case ARG_TYPE_REG8:
			sprintf(buf, "%s = 0x%02x", reg_strings[r.r8], state->registers[r.r8]);
			break;
		case ARG_TYPE_REG8_INDIRECT:
			sprintf(buf, "(%s) = 0x%02x", reg_strings[r.r8], state->registers[r.r8]);
			break;
		case ARG_TYPE_REG16:
			sprintf(buf, "%s = 0x%04x", reg16_strings[r.r16], state->registers16[r.r16]);
			break;
		case ARG_TYPE_REG16_INDIRECT:
			sprintf(buf, "(%s) = 0x%04x", reg16_strings[r.r16], state->registers16[r.r16]);
			break;
		case ARG_TYPE_DATA8:
			sprintf(buf, "0x%02x", state->arg);
			break;
		case ARG_TYPE_DATA8_UNSIGNED:
			sprintf(buf, "0x%02x", state->arg);
			break;
		case ARG_TYPE_DATA8_UNSIGNED_INDIRECT:
			sprintf(buf, "(0x%02x)", state->arg);
			break;
		case ARG_TYPE_DATA16:
			sprintf(buf, "0x%02x", state->arg);
			break;
		case ARG_TYPE_DATA16_UNSIGNED:
			sprintf(buf, "0x%02x", state->arg);
			break;
		case ARG_TYPE_DATA16_UNSIGNED_INDIRECT:
			sprintf(buf, "(0x%02x)", state->arg);
			break;
		case ARG_TYPE_REL8:
			rel = state->arg;
			sprintf(buf, "0x%04x", state->pc + *(int8_t*)&rel);
			break;
		case ARG_TYPE_REL8_ADD_SP:
			rel = state->arg;
			sprintf(buf, "0x%02x", state->sp + *(int8_t*)&rel);
			break;
		case ARG_TYPE_HL_INDIRECT_DEC:
			sprintf(buf, "(HL-) = 0x%04x", state->hl);
			break;
		case ARG_TYPE_HL_INDIRECT_INC:
			sprintf(buf, "(HL+) = 0x%04x", state->hl);
			break;
		X(00H);
		X(08H);
		X(10H);
		X(18H);
		X(20H);
		X(28H);
		X(30H);
		X(38H);
		X(0);
		X(1);
		X(2);
		X(3);
		X(4);
		X(5);
		X(6);
		X(7);
		X(Z);
		X(NC);
		X(NZ);
		case ARG_TYPE_NONE:
			buf[0] = 0;
			break;
	}
	
}
#undef X

void print_op(char *buffer, struct cpu_state *state, struct opcode *op)
{
	char arg0[1024];
	char arg1[1024];
	print_arg(arg0, state, op,op->arg0, op->i0);
	print_arg(arg1, state, op,op->arg1, op->i1);
	char sep;
	sep = op->arg1  == ARG_TYPE_NONE ? ' ' : ',';
	sprintf(buffer, "0x%04x %s %s %c %s", state->pc, op->name, arg0, sep, arg1);
}

#define X(n) state->memory->interrupt.n && state->memory->enabled.n
static int check_for_interrupts(struct cpu_state *state)
{
	//Check if interrupts are enabled
	reg16_t addr;
	if(state->memory->IME && (state->memory->IF & state->memory->IE & 0x1f))
	{
		state->memory->IME = 0;
		//Check each of the interrupts in priority order.
		if(X(v_blank))
		{
			state->memory->interrupt.v_blank = 0;
			addr = 0x0040;
		}
		else if(X(lcd_status))
		{
			state->memory->interrupt.lcd_status = 0;
			addr = 0x0048;
		}
		else if(X(timer))
		{
			state->memory->interrupt.timer = 0;
			addr = 0x0050;
		}
		else if(X(serial))
		{
			state->memory->interrupt.serial = 0;
			addr = 0x0058;
		}
		else if(X(joypad))
		{
			state->memory->interrupt.joypad = 0;
			addr = 0x0060;
		}
		Output("Interrupt 0x%04x\n", addr);
		cpu_push(state, state->pc);
		state->pc = addr;
	}
	return 0;
}
#undef X

uint8_t shade_table_0[4][3] =
{
	{255, 255, 255},
	{127,127,127},
	{63,63,63},
	{0,0,0},
};

void output_registers(struct cpu_state *state)
{
	static FILE *fp;
	if(!fp) fp = fopen("reg.txt", "w");
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "AF  = 0x%04x\n", state->af);
	fprintf(fp, "BC  = 0x%04x\n", state->bc);
	fprintf(fp, "DE  = 0x%04x\n", state->de);
	fprintf(fp, "HL  = 0x%04x\n", state->hl);
	fprintf(fp, "SP  = 0x%04x\n", state->sp);
	fprintf(fp, "PC  = 0x%04x\n\n", state->pc);

	fprintf(fp, "SCX = 0x%04x\n", state->memory->scx);
	fprintf(fp, "SCY = 0x%04x\n", state->memory->scy);
	fprintf(fp, "WX  = 0x%04x\n", state->memory->wx);
	fprintf(fp, "WY  = 0x%04x\n\n", state->memory->wy);

	fprintf(fp, "Bank  = 0x%04x\n\n", state->memory->current_bank);

	fprintf(fp, "LCDC:\n");
	fprintf(fp, "BG Display  : %u\n", state->memory->lcdc.bg_display);
	fprintf(fp, "OBJ Enable  : %u\n", state->memory->lcdc.obj_enable);
	fprintf(fp, "OBJ Size    : %u\n", state->memory->lcdc.obj_size);
	fprintf(fp, "Map Select  : %u\n", state->memory->lcdc.map_select);
	fprintf(fp, "Tile Select : %u\n", state->memory->lcdc.tile_select);
	fprintf(fp, "Window Disp : %u\n", state->memory->lcdc.window_display);
	fprintf(fp, "Window Map  : %u\n", state->memory->lcdc.window_map);
	fprintf(fp, "Enabled     : %u\n\n", state->memory->lcdc.enabled);


	fprintf(fp, "Interrupt Flags: (val) (enabled)\n");
	fprintf(fp, "IME      =         %u\n", state->memory->IME);
	fprintf(fp, "VBLANK   =         %u   %u \n",
						state->memory->interrupt.v_blank,
						state->memory->enabled.v_blank);
	fprintf(fp, "LCD STAT =         %u   %u \n",
						state->memory->interrupt.lcd_status,
						state->memory->enabled.lcd_status);
	fprintf(fp, "TIMER    =         %u   %u \n",
						state->memory->interrupt.timer,
						state->memory->enabled.timer);
	fprintf(fp, "SERIAL   =         %u   %u \n",
						state->memory->interrupt.serial,
						state->memory->enabled.serial);
	fprintf(fp, "JOYPAD   =         %u   %u \n",
						state->memory->interrupt.joypad,
						state->memory->enabled.joypad);
	fflush(fp);
}

void output_tile_maps(struct cpu_state *state)
{
	FILE *fp = fopen("tile_map.txt", "w");
	for(int i = 0; i < 32; i++)
	{
		for(int j = 0; j < 32; j ++)
		{
			fprintf(fp, "%02x ", state->memory->video_ram[0x1800 + 32 * i + j]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	for(int i = 0; i < 32; i++)
	{
		for(int j = 0; j < 32; j ++)
		{
			fprintf(fp, "%02x ", state->memory->video_ram[0x1C00 + 32 * i + j]);
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
	for(int j = 0; j < 8; j++)
	{
		for(int i = 0; i < 5; i++)
		{
			fprintf(fp,"%08x ", state->memory->OAM[j * 10 +  4]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

//TODO: Properly comment this.
#define GET_SHADE(x, n) shade_table_0[((x >> (2 * n)) & 0x3)]

struct cpu_state *g_state;
void sigabrt_handler(int x)
{
	int w = 32;
	int h = 6;
	ppm_t *ppm = ppm_new(w * 8, h * 8, "tiles.ppm");
	memory_t *mem = g_state->memory;
	for(int ty = 0; ty < h; ty++)
	{
		for(int tx = 0; tx < w; tx++)
		{
			//Tile map is located at address 0x9800 or 0x9c00
			int tile_num = ty * w + tx;
			uint8_t *tile_data = &mem->video_ram[tile_num * 16];
			for(int j = 0; j < 8; j++)
			{
				for(int i = 0; i < 8; i++)
				{
					uint8_t shade = ((tile_data[1] >> i) & 0x1) << 1 |
									((tile_data[0] >> i) & 0x1);
					uint8_t x = tx * 8 + (7 - i);
					uint8_t y = ty * 8 + j;
					#if 1
						uint8_t data[] = {64 * shade, 64 * shade, 64 * shade};
						ppm_write_pixel(ppm, x, y , data);
					#else
						ppm_write_pixel(ppm, x, y , GET_SHADE(mem->bgp, shade));
					#endif
				}
				tile_data += 2;
			}
		}
	}
	output_registers(g_state);
	output_tile_maps(g_state);
	exit(0);
}

void onexit_f(void)
{
	sigabrt_handler(0);
}

void cpu_start(struct cpu_state *state)
{
	g_state = state;
	signal(SIGABRT, sigabrt_handler);
	atexit(onexit_f);
	reg_t instruction;
	struct opcode *op;
	state->pc = 0;
	while(1)
	{
		//Reset status flags.
		state->success = 1;
		//Load instruction and execute it.
		check_for_interrupts(state);
		if(state->DI_Pending)
		{
			state->memory->IME = 0;
			state->DI_Pending = 0;
		}
		if(state->EI_Pending)
		{
			state->memory->IME = 1;
			state->EI_Pending = 0;
		}
		instruction = cpu_load8(state, state->pc);
		op          = &op_table[instruction];
		if(op->size == 2) state->arg = cpu_load8(state,  state->pc + 1);
		if(op->size == 3) state->arg = cpu_load16(state, state->pc + 1);
		state->pc  += op->size;

#if 1
		char buf[1024];
		print_op(buf, state, op);
		if(state->memory->boot_locked)
		{
			Output("%s\n", buf);
		}
		output_registers(state);
#endif
#if 1
#endif
		op->op(state, op->arg0, op->i0, op->arg1, op->i1);
		//Increment program counter.
		state->cycles += state->success ? op->success : op->fail;
		fflush(stdout);
	}
}

