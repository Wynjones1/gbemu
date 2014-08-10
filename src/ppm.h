#pragma once
#include <stdint.h>

typedef struct ppm ppm_t;

ppm_t *ppm_new(int width, int height, const char *filename);
void ppm_write_pixel(ppm_t *ppm, int width, int height, uint8_t *data);
