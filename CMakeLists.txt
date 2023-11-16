cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(project_name)
pico_sdk_init()

add_library(libs
    libs/init.c
    libs/panel.c
)

target_include_directories(libs PUBLIC
    libs/inc
)

target_link_libraries(libs
    pico_stdlib
    hardware_adc
    hardware_pwm
)

add_executable(solarTrackerProgramming
    main.c
)

target_link_libraries(solarTrackerProgramming pico_stdlib pico_time hardware_adc hardware_pwm libs)

pico_enable_stdio_usb(solarTrackerProgramming 1)
pico_enable_stdio_uart(solarTrackerProgramming 0)

pico_add_extra_outputs(solarTrackerProgramming)