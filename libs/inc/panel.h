#include "pico/stdlib.h"

extern float v_input_panel;
extern float v_max_searching;
extern bool after_searching_flag;
extern int current_position;
extern int panel_position_max_v;
extern uint8_t step_for_prop_control;
extern bool clockwise_moved;
extern bool anticlockwise_moved;

float calculate_input_panel_voltage();
void prop_control(uint8_t pwm_pin);
void first_movement_prop_control(uint8_t pwm_pin);
void move_panel(uint8_t pwm_pin, int new_position);
void search_of_max_voltage(uint8_t pwm_pin);