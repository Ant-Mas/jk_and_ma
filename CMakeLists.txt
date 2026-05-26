# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(jk_and_ma C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(jk_and_ma 
    jk_and_ma.c 
    OLED_I2C.c
    encoder.c
    notes.c
    loop.c
    usb_descriptors.c
    )

pico_set_program_name(jk_and_ma "jk_and_ma")
pico_set_program_version(jk_and_ma "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(jk_and_ma 0)
pico_enable_stdio_usb(jk_and_ma 0)

# Add the standard include files to the build
target_include_directories(jk_and_ma PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(jk_and_ma 
        pico_stdlib
        tinyusb_device
        tinyusb_board
        hardware_i2c
        hardware_timer
        hardware_pwm
        hardware_irq
        pico_multicore
        )

pico_add_extra_outputs(jk_and_ma)

