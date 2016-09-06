#pragma once
#include "types.h"
/* Forward declare cpu struct*/
typedef struct cpu_state cpu_state_t;

/* These macros define each of the sound registers, the form of X is field_name, msb, lsb
   this allows us to repeat the registers in different contexts. 
*/
// Square 1
#define NR10               \
    X(shift,        2, 0)  \
    X(negate,       3, 3)  \
    X(sweep_period, 6, 4)
#define NR11               \
    X(load_len,     5, 0)  \
    X(duty,         7, 6)  
#define NR12               \
    X(period,       2, 0)  \
    X(add_mode,     3, 3)  \
    X(start_volume, 7, 4)
#define NR13               \
    X(freq_lsb,     7, 0)  
#define NR14               \
    X(freq_msb,     2, 0)  \
    X(len_en,       6, 6)  \
    X(trigger,      7, 7)
// Square 2
#define NR21               \
    X(load_len,     5, 0)  \
    X(duty,         7, 6)  
#define NR22               \
    X(period,       2, 0)  \
    X(add_mode,     3, 3)  \
    X(start_volume, 7, 4)
#define NR23               \
    X(freq_lsb,     7, 0)  
#define NR24               \
    X(freq_msb,     2, 0)  \
    X(len_en,       6, 6)  \
    X(trigger,      7, 7)
// Wave
#define NR30               \
    X(power,        7, 7)  
#define NR31               \
    X(load_len,     7, 0)  
#define NR32               \
    X(volume_code,  6, 5)  
#define NR33               \
    X(freq_lsb,     7, 0)  
#define NR34               \
    X(freq_msb,     2, 0)  \
    X(len_en,       6, 6)  \
    X(trigger,      7, 7)  
// Noise
#define NR41               \
    X(load_len,     5, 0)  
#define NR42               \
    X(period,       2, 0)  \
    X(add_mode,     3, 3)  \
    X(start_volume, 7, 4) 
#define NR43               \
    X(divisor,      2, 0)  \
    X(width_mode,   3, 3)  \
    X(clock_shift,  7, 4) 
#define NR44               \
    X(len_en,       6, 6)  \
    X(trigger,      7, 7)  
// Control / Status
#define NR50               \
    X(r_vol,        2, 0)  \
    X(vin_r_en,     3, 3)  \
    X(l_vol,        6, 4)  \
    X(vin_l_en,     7, 7)
#define NR51               \
    X(r_en,         3, 0)  \
    X(l_en,         7, 4)
#define NR52               \
    X(channel_stat, 3, 0)  \
    X(power,        7, 7)

#define NOISE_BUFFER_SIZE (44100)

typedef struct audio
{
	cpu_state_t *state;
/* Refer to the macros at the top of this file for the definitions of these registers */
#define X(name, msb, lsb) uint8_t name;
    struct
    {
        NR10
        NR11
        NR12
        NR13
        NR14
        int8_t volume;
        uint8_t en;
		uint8_t envelope_counter;
    }sq1;

    struct
    {
        NR21
        NR22
        NR23
        NR24
        int8_t volume;
        uint8_t en;
		uint8_t envelope_counter;
    }sq2;

    struct
    {
        NR30
        NR31
        NR32
        NR33
        NR34
        int8_t  volume;
        uint8_t en;
		uint8_t envelope_counter;
    }wave;

    struct
    {
        NR41
        NR42
        NR43
        NR44
        int8_t  volume;
        uint8_t en;
		uint8_t envelope_counter;
    }noise;

    struct
    {
        NR50
        NR51
        NR52
    }control;

#undef X
	uint8_t  wave_table[32];
	uint32_t frame_sequencer_step;
	uint32_t frame_sequencer_count;
	uint16_t sweep_count;
	uint32_t buffer_pos;
}audio_t;

audio_t *audio_init(cpu_state_t *state);
reg_t    audio_load(audio_t *audio, reg16_t addr);
void     audio_simulate(audio_t *audio, int clk);
void     audio_store(audio_t *audio, reg16_t addr, reg_t data);
void     audio_store_wave_sample(audio_t *audio, uint8_t idx, reg_t samples);
void     audio_delete(audio_t *audio);
uint32_t noise_freq(audio_t *audio);
uint32_t noise_clk_count(audio_t *audio);
