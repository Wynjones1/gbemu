#pragma once
#include "types.h"
typedef struct cpu_state cpu_state_t;

typedef struct audio
{
	cpu_state_t *state;
    int shift;
    int negate;
    int sweep_period;
    int load_len;
    int duty;
    int period;
    int add_mode;
    int start_volume;
    int volume;
    int freq;
    int len_en;
    int en;
}audio_t;

void     audio_start_thread(void);
audio_t *audio_init(cpu_state_t *state);
reg_t    audio_load(audio_t *audio, reg16_t addr);
void     audio_simulate(audio_t *audio, int clk);
void     audio_store(audio_t *audio, reg16_t addr, reg_t data);
void     audio_delete(audio_t *audio);
