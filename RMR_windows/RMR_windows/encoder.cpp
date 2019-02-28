#include "encoder.h"
#include "pch.h"


void encoder_init_values(Encoder* enc, uint16_t data)
{
	enc->reset_state = data;
	enc->encoder_current_value = data;
	enc->encoder_last_value = data;

}

void encoder_process(Encoder* enc, uint16_t encoder_data)
{
	enc->encoder_current_value = encoder_data;

	if ((enc->encoder_current_value - enc->encoder_last_value) < -encoder_max_value / 2)
		enc->owerflow_flounter++;

	else if ((enc->encoder_current_value - enc->encoder_last_value) > encoder_max_value / 2)
		enc->owerflow_flounter--;

	enc->encoder_real_value = encoder_max_value * enc->owerflow_flounter + enc->encoder_current_value - enc->reset_state;

	enc->encoder_last_value = enc->encoder_current_value;

}