#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include <stdio.h>

void initialize_adc(uint8_t adc_pin, uint8_t adc_input);
void initialize_pwm(uint8_t pwm_pin);
int get_position_with_degrees(float degree);
float get_degrees_with_position(int position);
