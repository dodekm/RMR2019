#pragma once
#include <cstdint>

#define encoder_max_value  65535 


typedef struct  Encoder
{
	uint16_t encoder_current_value = 0;
	uint16_t encoder_last_value = 0;
	int encoder_real_value = 0;
	int owerflow_counter = 0;
	int	reset_state = 0;

}Encoder;


void encoder_process(Encoder*, uint16_t);
void encoder_init_values(Encoder*, uint16_t);