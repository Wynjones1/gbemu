#pragma once
#include "types.h"
typedef struct cpu_state cpu_state_t;

typedef struct audio
{
	cpu_state_t *state;
	union
	{
		struct
		{
			union
			{
				struct
				{
					uint16_t shift    :  3; //NR10
					uint16_t negate   :  1;
					uint16_t sweep    :  3;
					uint16_t          :  1;
					uint16_t len_load :  6; //NR11
					uint16_t duty     :  2;
                    uint16_t period   :  3; //NR12
                    uint16_t env_add  :  1;
                    uint16_t volume   :  4;
					uint16_t freq     : 11; //NR13-14
					uint16_t          :  3;
					uint16_t len_en   :  1;
					uint16_t trigger  :  1;
				};
				struct
				{
					uint8_t NR10;
					uint8_t NR11;
					uint8_t NR12;
					uint8_t NR13;
					uint8_t NR14;
				};
			}square1;

			union
			{
				struct
				{
					uint16_t           : 8;
					uint16_t len_load  : 6; //NR21
					uint16_t duty      : 2;
					uint16_t period    : 3; //NR22
					uint16_t add_mode  : 1;
					uint16_t start_vol : 4;
					uint16_t freq      : 11;//NR23-24
					uint16_t           : 3;
					uint16_t len_en    : 1;
					uint16_t trigger   : 1;
				};
				struct
				{
					uint8_t _unused_;
					uint8_t NR21;
					uint8_t NR22;
					uint8_t NR23;
					uint8_t NR24;
				};
			}square2;

			union
			{
				struct
				{
					uint8_t          : 7;
					uint8_t DAC      : 1;
					uint8_t len_load : 8;
					uint8_t          : 5;
					uint8_t vol_code : 2;
					uint8_t          : 1;
					uint8_t freq_lsb : 8;
					uint8_t freq_msb : 3;
					uint8_t          : 3;
					uint8_t len_en   : 1;
					uint8_t trigger  : 1;
				};
				struct
				{
					uint8_t NR30;
					uint8_t NR31;
					uint8_t NR32;
					uint8_t NR33;
					uint8_t NR34;
				};
			}wave;

			union
			{
				struct
				{
					uint8_t            : 8;
					uint8_t len_load   : 6;
					uint8_t            : 2;
					uint8_t period     : 3;
					uint8_t add_mode   : 1;
					uint8_t start_vol  : 4;
					uint8_t div_code   : 3;
					uint8_t width_mode : 1;
					uint8_t clk_shift  : 4;
					uint8_t            : 6;
					uint8_t len_en     : 1;
					uint8_t trigger    : 1;
				};
				struct
				{
					uint8_t unused;
					uint8_t NR41;
					uint8_t NR42;
					uint8_t NR43;
					uint8_t NR44;
				};
			}noise;

			union
			{
				struct
				{
					uint8_t right_vol   : 3;
					uint8_t r_en        : 1;
					uint8_t left_vol    : 3;
					uint8_t l_en        : 1;
					uint8_t nw21_r      : 8;
					uint8_t nw21_l      : 8;
					uint8_t chan_len    : 4;
					uint8_t             : 3;
					uint8_t pwr_control : 1;
				};
				struct
				{
					uint8_t NR50;
					uint8_t NR51;
					uint8_t NR52;
				};
			};
			uint8_t unused[2];
			uint8_t sample_table[0xf];
		};
		uint8_t registers[0xff3f - 0xff10];
	};
    int channel_counters[4];
    int channel_en[4];
    int sweep_timer;
}audio_t;

void     audio_start_thread(void);
audio_t *audio_init(cpu_state_t *state);
reg_t    audio_load(audio_t *audio, reg16_t addr);
void     audio_simulate(audio_t *audio, int clk);
void     audio_store(audio_t *audio, reg16_t addr, reg_t data);
void     audio_delete(audio_t *audio);
