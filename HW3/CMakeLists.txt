cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(MARK_PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(cdc)

target_sources(cdc PRIVATE cdc.c)

pico_enable_stdio_usb(cdc 1)
pico_enable_stdio_uart(cdc 0)

target_link_libraries(cdc pico_stdlib hardware_adc)




# create map/bin/hex file etc.
pico_add_extra_outputs(cdc)






