#include <stdio.h>
#include "panel.h"
#include "init.h"
#include "pico/time.h"


int main(void) {
    // Set pins
	const uint8_t ADC_PIN = 26;
	const uint8_t ADC_INPUT = 0;
    const uint8_t PWM_PIN = 2;

    stdio_init_all();

    // Initialize ADC
    initialize_adc(ADC_PIN, ADC_INPUT);

    // Initialize PWM
    initialize_pwm(PWM_PIN);
    
	bool need_of_searching = true;
    absolute_time_t t0 = get_absolute_time();

    while (true) {
        
        
        // If the degree isn't inside the angles possible, then the panel should go to 90 degrees
		if (get_degrees_with_position(current_position) < 0 || get_degrees_with_position(current_position) > 90) {
			move_panel(PWM_PIN, get_position_with_degrees(45));
			need_of_searching = true;
		}
        
        
        // Do the searching every 30 minutes
        
        absolute_time_t t1 = get_absolute_time();
        
        // Set the time in which the searching should be executed
        if (absolute_time_diff_us(t0, t1) >= 3600000000) {
            need_of_searching = true;
        }

        
        if (need_of_searching == true) {
            search_of_max_voltage(PWM_PIN);
            t0 = get_absolute_time();
            // Move the panel to the position with the maximum voltage
            move_panel(PWM_PIN, panel_position_max_v);

			need_of_searching = false;
        }

        // After searching the maximum the panel will make the proportional control
        prop_control(PWM_PIN);

        sleep_ms(750);
    }

    return 0;
}