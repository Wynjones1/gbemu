#pragma once
#include "cpu.h"

typedef struct audio
{
	cpu_state_t *state;
}audio_t;

void audio_start_thread(void);
int audio_init(void);
