#include "init.h"

void initialize_adc(uint8_t adc_pin, uint8_t adc_input) {
	// Initialize ADC
	adc_init();
	adc_gpio_init(adc_pin);
	adc_select_input(adc_input);
}

void initialize_pwm(uint8_t pwm_pin) {
    // Initialize PWM
    gpio_set_function(pwm_pin, GPIO_FUNC_PWM);
    uint8_t slice = pwm_gpio_to_slice_num(pwm_pin);

    pwm_config config = pwm_get_default_config();

    // Divide clock in 50
    pwm_config_set_clkdiv(&config, 50.0);

    // Divide the output in 50000 posibilities
    pwm_config_set_wrap(&config, 50000);

    // pwm_config_set_output_polarity(&config, true, false);

    pwm_init(slice, &config, false);

    pwm_set_gpio_level(pwm_pin, get_position_with_degrees(0));

    pwm_set_enabled(slice, true);
}

int get_position_with_degrees(float degree) {
    // Calculate position with degrees
	int position = degree / 180 * 5000 + 1250;
	return position;
}

float get_degrees_with_position(int position) {
    // Calculate degrees with position
    float degree = ((float) position - 1250) * 180 / 5000;
    return degree;
}