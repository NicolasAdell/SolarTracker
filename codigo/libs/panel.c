#include "init.h"
#include "panel.h"

// Set variables to 0
float v_input_panel = 0;
float v_max_searching = 0;
bool after_searching_flag = false;
int current_position = 1250;
int panel_position_max_v = 1250;
uint8_t step_for_prop_control = 60;
bool clockwise_moved = false;
bool anticlockwise_moved = false;


float calculate_input_panel_voltage() {
    // Read tension of the ADC
    uint16_t adc_value = adc_read();

    // Calculate voltage from adc
    float voltage = adc_value * 3.3 / 4095;

    // Calculate input voltage from panel
    float v_input_panel = voltage * (10000 + 2200) / 2200;
    return v_input_panel;
}


void prop_control(uint8_t pwm_pin) {
    v_input_panel = 0;

    // Check if the panel has recently turned
    if (after_searching_flag == true) {
        first_movement_prop_control(pwm_pin);
        after_searching_flag = false;
    }

    // Calculate voltage in the current position
    v_input_panel = calculate_input_panel_voltage();
    // Calculate the error
    float error_calc = v_max_searching - v_input_panel;

    printf("Tension max = %f\n", v_max_searching);
    printf("Pos tension max = %i\n", panel_position_max_v);
    printf("Pos actual = %d\n", current_position);
    printf("Tension actual = %f\n", v_input_panel);
    printf("---------------\n");
    

	// If error is greater, then return to the original position
    if (error_calc >= 0) {
        printf("Tension menor al maximo\n");
        
        move_panel(pwm_pin, panel_position_max_v);
        
        v_max_searching = calculate_input_panel_voltage();

        printf("Nueva tension max = %f\n", v_max_searching);
        printf("Nueva pos tension max = %i\n", panel_position_max_v);
        printf("Nueva pos actual = %d\n", current_position);
        printf("---------------\n");

        sleep_ms(30000000);
    } else if (error_calc < 0) {
        
        printf("Tension mayor al maximo\n");
        
		// If error is lower, then a new maximum is set
        panel_position_max_v = current_position;
        v_max_searching = v_input_panel;
    }
        
    // It should alternate between clock and anticlockwise, despite having an extreme angle

    if (anticlockwise_moved == true || (get_degrees_with_position(current_position) >= (90 - get_degrees_with_position(step_for_prop_control)))) {
        move_panel(pwm_pin, current_position - step_for_prop_control);
        anticlockwise_moved = false;
        clockwise_moved = true;
    } else if (clockwise_moved == true || (get_degrees_with_position(current_position) <= (0 + get_degrees_with_position(step_for_prop_control)))) {
        move_panel(pwm_pin, current_position + step_for_prop_control);
        anticlockwise_moved = true;
        clockwise_moved = false;
    }
}

void first_movement_prop_control(uint8_t pwm_pin) {
    // Save current position
    panel_position_max_v = current_position;
    v_max_searching = calculate_input_panel_voltage();

    printf("Primer tension: %f", v_max_searching);

    // Turn it one position to start the proportional control
    if (get_degrees_with_position(current_position) >= 0 && get_degrees_with_position(current_position) <= 45) {
        move_panel(pwm_pin, current_position + step_for_prop_control);
        anticlockwise_moved = true;
        clockwise_moved = false;
    } else if (get_degrees_with_position(current_position) > 45 && get_degrees_with_position(current_position) <= 90) {
        move_panel(pwm_pin, current_position - step_for_prop_control);
        clockwise_moved = true;
        anticlockwise_moved = false;
    }
}

void move_panel(uint8_t pwm_pin, int desired_position) {
    
    // Check if the new position is possible
    if (get_degrees_with_position(desired_position) < 0 || get_degrees_with_position(desired_position) > 90 
        || desired_position == current_position) {
        return;
    }

    // Move one position at a time to reach the disired position 
    while (current_position != desired_position) {

        int new_position;
        if (desired_position - current_position > 0) {
            new_position = current_position + 1;
            pwm_set_gpio_level(pwm_pin, new_position);
        } else if (desired_position - current_position < 0) {
            new_position = current_position - 1;
            pwm_set_gpio_level(pwm_pin, new_position);
        }

        current_position = new_position;
        sleep_ms(5);
    }
}

void search_of_max_voltage(uint8_t pwm_pin) {

    // Set the variables in 0 degrees
	int pos_variable = get_position_with_degrees(0);
    v_max_searching = 0;
    panel_position_max_v = get_position_with_degrees(0);

    // The loop continues up to 180 degrees
	do {
        // Move the panel to the new position
		move_panel(pwm_pin, pos_variable);
        // Calculate the input voltage
		v_input_panel = calculate_input_panel_voltage();
        // If it's grater than the last measure, the variable will be saved
        if (v_input_panel > v_max_searching) {
            v_max_searching = v_input_panel;
            panel_position_max_v = current_position;
        }

        // Next position is set
		pos_variable++;
    } while (((get_degrees_with_position(pos_variable)) < 90));
    
    printf("Despues de buscar el maximo\n");
    printf("Posicion actual: %d\n", current_position);
    printf("Tension max = %f\n", v_max_searching);
    printf("Pos tension max = %i\n", panel_position_max_v);
    printf("Tension actual = %f\n", v_input_panel);
    printf("---------------\n");

    // Set the flag when it's finished
    after_searching_flag = true;
}