cmake_minimum_required(VERSION 3.16)

SET(FREERTOS_KERNEL_PATH "$ENV{FREERTOS_PATH}")


include(pico_sdk_import.cmake) # include the PICO_SDK
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(app)

pico_sdk_init()

add_executable(
    ${PROJECT_NAME}_m
    ./src/master.c
    ./src/spi.c
    ./src/utils.c
    ./src/packet.c
    ./src/dht.c
)

add_executable(
    ${PROJECT_NAME}_s 
    ./src/slave.c
    ./src/spi.c
    ./src/utils.c
    ./src/packet.c
    ./src/dht.c
    ./src/ky037.c

)
#

target_include_directories(${PROJECT_NAME}_m PRIVATE ./inc)
target_include_directories(${PROJECT_NAME}_s PRIVATE ./inc)


# Link with the pico stdlib
target_link_libraries(${PROJECT_NAME}_m pico_stdlib hardware_adc hardware_spi hardware_adc FreeRTOS-Kernel-Heap4)
target_link_libraries(${PROJECT_NAME}_s pico_stdlib hardware_adc hardware_spi hardware_adc FreeRTOS-Kernel-Heap4)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME}_m)
pico_add_extra_outputs(${PROJECT_NAME}_s)


# Enable USB serial
pico_enable_stdio_usb(${PROJECT_NAME}_m   1)
pico_enable_stdio_uart(${PROJECT_NAME}_m  0)

pico_enable_stdio_usb(${PROJECT_NAME}_s   1)
pico_enable_stdio_uart(${PROJECT_NAME}_s  0)