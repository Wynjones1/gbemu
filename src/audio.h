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
			uint8_t shift    : 3;
			uint8_t negate   : 1;
			uint8_t sweep    : 3;
			uint8_t          : 1;
			uint8_t len_load : 6;
			uint8_t duty     : 2;
			uint8_t freq_lsb : 8;
			uint8_t freq_msb : 3;
			uint8_t          : 3;
			uint8_t len_en   : 1;
			uint8_t trigger  : 1;
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
			uint8_t           : 8;
			uint8_t len_load  : 6;
			uint8_t duty      : 2;
			uint8_t period    : 3;
			uint8_t add_mode  : 1;
			uint8_t start_vol : 4;
			uint8_t freq_lsb  : 8;
			uint8_t freq_msb  : 3;
			uint8_t           : 3;
			uint8_t len_en    : 1;
			uint8_t trigger   : 1;
		};
		struct
		{
			uint8_t  : 8;
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

	uint8_t sample_table[0xf];
}audio_t;

void     audio_start_thread(void);
audio_t *audio_init(cpu_state_t *state);
reg_t    audio_load(audio_t *audio, reg16_t addr);
void     audio_store(audio_t *audio, reg16_t addr, reg_t data);
void     audio_delete(audio_t *audio);
