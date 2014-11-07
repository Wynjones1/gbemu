#include "rom.h"
#include <stdio.h>
#include "common.h"

char nintendo_logo[] =
{
  0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
  0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
  0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
};

static int verify(FILE *fp)
{
	char to_verify[sizeof(nintendo_logo)];
	fseek(fp, 0x104, SEEK_SET);
	common_fread(to_verify, 1, sizeof(nintendo_logo), fp);
	for(int i = 0; i < sizeof(nintendo_logo); i++)
	{
		if(to_verify[i] != nintendo_logo[i]) return 0;
	}
	return 1;
}

rom_t rom_read(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	fseek(fp, 0x0134, SEEK_SET);
	rom_t out;
	common_fread(out.title,             1, 0x144 - 0x134, fp); // 0x134 - 0x143
	common_fread(out.licensee_code,     1, 0x146 - 0x144, fp); // 0x144 - 0x145
	common_fread(&out.sgb_flag,         1, 1,             fp); // 0x146
	common_fread(&out.cart_type,        1, 1,             fp); // 0x147
	common_fread(&out.rom_size,         1, 1,             fp); // 0x148
	common_fread(&out.ram_size,         1, 1,             fp); // 0x149

	out.title[15]             = 0;
	out.manufacturer_code[4]  = 0;
	out.licensee_code[2]      = 0;

	printf("Game title        : %s\n",       out.title);
	/*
	printf("Manufacturer code : %s\n",       out.manufacturer_code);
	printf("Licensee code     : %s\n",       out.licensee_code);
	*/
	printf("Cart type         : 0x%x\n",     out.cart_type);
	printf("ROM size          : %dKBytes\n", 32 <<  out.rom_size );
	printf("RAM size          : %d\n",       out.ram_size );
	printf("Game OK           : %d\n",       verify(fp));
	fclose(fp);
	return out;
}
