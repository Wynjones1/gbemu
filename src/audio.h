#pragma once
#include "types.h"
typedef struct cpu_state cpu_state_t;

#define NR10              \
    X(shift,        2, 0) \
    X(negate,       3, 3) \
    X(sweep_period, 6, 4)

#define NR11              \
    X(load_len,     5, 0) \
    X(duty,         7, 6) \

#define NR12              \
    X(period,       2, 0) \
    X(add_mode,     3, 3) \
    X(start_volume, 7, 4)

#define NR13              \
    X(freq_lsb,     7, 0) \

#define NR14              \
    X(freq_msb,     2, 0) \
    X(len_en,       6, 6) \
    X(trigger,      7, 7)

#define NR21              \
    X(load_len,     5, 0) \
    X(duty,         7, 6) \

#define NR22              \
    X(period,       2, 0) \
    X(add_mode,     3, 3) \
    X(start_volume, 7, 4)

#define NR23              \
    X(freq_lsb,     7, 0) \

#define NR24              \
    X(freq_msb,     2, 0) \
    X(len_en,       6, 6) \
    X(trigger,      7, 7)

typedef struct audio
{
	cpu_state_t *state;
#define X(name, msb, lsb) uint8_t name;// : ((msb - lsb) + 1);
    struct{
        NR10
        NR11
        NR12
        NR13
        NR14
        int volume;
        int en;
    }sq1;

    struct{
        NR21
        NR22
        NR23
        NR24
        int volume;
        int en;
    }sq2;
#undef X
}audio_t;

void     audio_start_thread(void);
audio_t *audio_init(cpu_state_t *state);
reg_t    audio_load(audio_t *audio, reg16_t addr);
void     audio_simulate(audio_t *audio, int clk);
void     audio_store(audio_t *audio, reg16_t addr, reg_t data);
void     audio_delete(audio_t *audio);

