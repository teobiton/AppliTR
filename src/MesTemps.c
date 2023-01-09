/*
 * MesTemps.c
 *
 * Created: 16/12/2019 00:48:08
 *  Author: pasquier-o
 */ 
#include "tc.h"

//Function prototype definition
void ConfigureMeasure(void);
void StartMeasure(void);
uint16_t EndMeasure(void);

// creation de variables globales
struct tc_module tc_instance;

// configuration du timer 4
void ConfigureMeasure(void)
{
	struct tc_config config_tc;

	tc_get_config_defaults(&config_tc);

	config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
	config_tc.wave_generation = TC_WAVE_GENERATION_MATCH_FREQ;
	config_tc.clock_source = GCLK_GENERATOR_0;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV8;
	config_tc.counter_16_bit.compare_capture_channel[0] = 0xFFFF;

	tc_init(&tc_instance, TC4, &config_tc);
	tc_enable(&tc_instance);
}

void StartMeasure(void)
{
	tc_set_count_value(&tc_instance, 0);
}

uint16_t EndMeasure(void)
{
	volatile uint16_t Measure;
	
	Measure = (uint16_t) tc_get_count_value(&tc_instance);
	return Measure;
}
