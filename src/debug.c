#include "debug.h"
#include "ppm.h"
#include "common.h"
#include "memory.h"
#include <signal.h>

struct cpu_state *g_state;

void sigabrt_handler(int x)
{
	debug_on_exit();
}

void debug_on_exit(void)
{
    if(OUTPUT_DEBUG_FILES)
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
        debug_output_registers(g_state);
        debug_output_tile_maps(g_state);
        debug_output_framebuffer(g_state);
    }
	cpu_delete(g_state);
	exit(0);
}

void output_tiles(void)
{
	struct stat st = {};
	if(stat("./tiles/", &st) == -1)
	{
		common_mkdir("./tiles/", 0700);
	}
	int w = 32;
	int h = 6;
	ppm_t *full = ppm_new(w * 8, h * 8, "./tiles/out.ppm");
	memory_t *mem = g_state->memory;
	//Loop through each of the tiles.
	for(int ty = 0; ty < h; ty++)
	{
		for(int tx = 0; tx < w; tx++)
		{
			//Tile map is located at address 0x9800 or 0x9c00
			char buf[1024];
			sprintf(buf, "./tiles/%03d.ppm", ty * w + tx);
			ppm_t *ppm = ppm_new(8, 8, buf);
			int tile_num = ty * w + tx;
			uint8_t *tile_data = &mem->video_ram[tile_num * 16];
			//Output the tile
			for(int j = 0; j < 8; j++)
			{
				for(int i = 0; i < 8; i++)
				{
					uint8_t shade = ((tile_data[1] >> i) & 0x1) << 1 |
									((tile_data[0] >> i) & 0x1);
					uint8_t x = tx * 8 + (7 - i);
					uint8_t y = ty * 8 + j;
					uint8_t data[] = {64 * shade, 64 * shade, 64 * shade};
					ppm_write_pixel(ppm, 7 - i, j , data);
					ppm_write_pixel(full, x, y , data);
				}
				tile_data += 2;
			}
		}
	}
}

uint8_t tab[] =
{
	0, 128, 196, 255
};

void debug_output_framebuffer(struct cpu_state *state)
{
	ppm_t *ppm = ppm_new(256, 256, "framebuffer.ppm");
	for(int i = 0; i < 32; i++)
	{
		for(int j = 0; j < 32; j++)
		{
			for(int o = 0; o < 8; o++)
			{
				const uint8_t *data = memory_get_tile_data(state->memory, i, j, o, state->memory->lcdc.map_select);
				for(int k = 0; k < 8;k++)
				{
					uint8_t shade = tab[display_get_shade(data, k)];
					uint8_t pixel[] = {shade, shade, shade};
					ppm_write_pixel(ppm,  8 * i + k, 8 * j + o, pixel);
				}
			}
		}
	}
}

void debug_output_tile_maps(struct cpu_state *state)
{
	FILE *fp = common_fopen("tile_map.txt", "w");
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

uint8_t shade_table_0[4][3] =
{
	{255, 255, 255},
	{127,127,127},
	{63,63,63},
	{0,0,0},
};

void debug_output_registers(struct cpu_state *state)
{
	static FILE *fp;
	if(!fp) fp = common_fopen("reg.txt", "w");
	fseek(fp, 0, SEEK_SET);
	FOutput(fp, "AF  = 0x%04x   ", state->af);
	FOutput(fp, "BC  = 0x%04x\n", state->bc);
	FOutput(fp, "DE  = 0x%04x   ", state->de);
	FOutput(fp, "HL  = 0x%04x\n", state->hl);
	FOutput(fp, "SP  = 0x%04x   ", state->sp);
	FOutput(fp, "PC  = 0x%04x\n\n", state->pc);

	FOutput(fp, "SCX = 0x%04x   " , state->memory->scx);
	FOutput(fp, "SCY = 0x%04x\n"  , state->memory->scy);
	FOutput(fp, "LY  = 0x%04x   " , state->memory->ly);
	FOutput(fp, "LXC = 0x%04x\n"  , state->memory->lyc);
	FOutput(fp, "WX  = 0x%04x   " , state->memory->wx);
	FOutput(fp, "WY  = 0x%04x\n\n", state->memory->wy);

	FOutput(fp, "Bank  = 0x%04x\n\n", state->memory->current_bank);

	FOutput(fp, "LCDC:\n");
	FOutput(fp, "BG Display  : %u   ", state->memory->lcdc.bg_display);
	FOutput(fp, "OBJ Enable  : %u\n", state->memory->lcdc.obj_enable);
	FOutput(fp, "OBJ Size    : %u   ", state->memory->lcdc.obj_size);
	FOutput(fp, "Map Select  : %u\n", state->memory->lcdc.map_select);
	FOutput(fp, "Tile Select : %u   ", state->memory->lcdc.tile_data_select);
	FOutput(fp, "Window Disp : %u\n", state->memory->lcdc.window_display);
	FOutput(fp, "Window Map  : %u   ", state->memory->lcdc.window_map);
	FOutput(fp, "Enabled     : %u\n\n", state->memory->lcdc.enabled);


	FOutput(fp, "Interrupt Flags (val/enabled):\n");
	FOutput(fp, "IME      : %u\n", state->memory->IME);
    #define X(flag) FOutput(fp, #flag " : %u/%u \n", \
        BIT_N(state->memory->IF, flag ## _BIT),      \
        BIT_N(state->memory->IE, flag ## _BIT))
	X(VBLANK);
	X(LCD_STATUS);
	X(TIMER);
	X(SERIAL);
	X(JOYPAD);
    #undef X


	FOutput(fp, "Joypad\n");
	FOutput(fp, "a     = %d b      = %d\n", state->memory->buttons.a, state->memory->buttons.b);
	FOutput(fp, "start = %d select = %d\n", state->memory->buttons.start , state->memory->buttons.select);
	FOutput(fp, "up    = %d down   = %d\n", state->memory->dpad.up, state->memory->dpad.down);
	FOutput(fp, "left  = %d right  = %d\n", state->memory->dpad.left , state->memory->dpad.right);
	FOutput(fp, "buttons = %d dpad = %d\n", state->memory->buttons.enabled , state->memory->dpad.enabled);
	fflush(fp);
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
void debug_print_arg(char *buf, struct cpu_state *state, struct opcode *op,enum ARG_TYPE arg, REG_INPUT r)
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
			sprintf(buf, "0x%04x", state->pc + *(int8_t*)&rel + 2);
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

void debug_print_op(char *buffer, struct cpu_state *state, struct opcode *op)
{
	char arg0[1024];
	char arg1[1024];
	if(op->op == PREFIX_CB)
	{
		op = &cb_op_table[state->arg];
	}
	debug_print_arg(arg0, state, op,op->arg0, op->i0);
	debug_print_arg(arg1, state, op,op->arg1, op->i1);
	char sep;
	sep = op->arg1  == ARG_TYPE_NONE ? ' ' : ',';
	sprintf(buffer, "0x%04x %s %s %c %s", state->pc - op->size, op->name, arg0, sep, arg1);
}


void BREAK(void)
{
	raise(SIGINT);
}

void BREAKIF(int cond)
{
	if(cond)
	{
		raise(SIGINT);
	}
}

int *break_addresses;
int  num_break_addresses;

void debug_init(void)
{
    const char *filename = cmdline_args.break_file;
    if(filename)
    {
        char buf[50];
        FILE *fp = FOPEN(filename,"r");
        int bank;
        int pc;
        int count = 0;
        int *out = NULL;
        fgets(buf, 49, fp);
        while(!feof(fp))
        {
            out = realloc(out, sizeof(int) * 2 * (count + 1));
            if(sscanf(buf, "%X:%X", &bank, &pc) == 2)
            {
                out[2 * count]     = bank;
                out[2 * count + 1] = pc;
                count += 1;
            }
            else if(sscanf(buf, "%X", &pc) == 1)
            {
                out[2 * count]     = -1;
                out[2 * count + 1] = pc;
                count += 1;
            }
            fgets(buf, 49, fp);
        }
        break_addresses     = out;
        num_break_addresses = count;
    }
}

bool debug_is_break_address(int current_bank, uint16_t pc)
{
    for(int i = 0;i < num_break_addresses;i++)
    {
        int      break_bank = break_addresses[2 * i];
        uint16_t break_pc   = break_addresses[2 * i + 1];
        if((break_bank == -1 || break_bank == 0 || break_bank == current_bank) && (break_pc == pc))
            return true;
    }
    return false;
}
