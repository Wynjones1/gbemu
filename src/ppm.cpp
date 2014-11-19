#include "ppm.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>

struct ppm
{
	FILE  *fp;
	long   pos;
	int    width;
	int    height;
};

ppm_t *ppm_new(int width, int height, const char *filename)
{
	ppm_t *out  = (ppm_t*) malloc(sizeof(ppm_t));
	out->fp     = FOPEN(filename, "w");
	out->width  = width;
	out->height = height;

	fprintf(out->fp, "P6\n%d %d\n255\n", width, height);
	out->pos = ftell(out->fp);
	fseek(out->fp, out->pos + 3 * width * height - 1, SEEK_SET);
	fputc(0, out->fp);
	return out;
}

void ppm_write_pixel(ppm_t *ppm, int x, int y, uint8_t *data)
{
	fseek(ppm->fp, ppm->pos + 3 * (ppm->width * y + x), SEEK_SET);
	fwrite(data, 1, 3, ppm->fp);
	fflush(ppm->fp);
}
