cmake_minimum_required(VERSION 3.12)
project(DustSensor C CXX)
set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(/opt/arm/stm32/inc)
include_directories(/opt/arm/stm32/CMSIS/Include)
include_directories(/opt/arm/stm32/CMSIS/Device/ST/STM32F4xx/Include)

set(THIRD_PARTY src/startup/startup_stm32.c src/utils/delay.c src/utils/gpio.c src/sbrk.c)
set(DRIVERS src/drivers/uart.c src/drivers/adc.c src/drivers/timers.c src/drivers/dust-sensor.c)
set(APPLICATION src/application/logger.c src/application/multi-measure.c src/application/app.c include/app.h)

add_executable(${PROJECT_NAME}.elf src/main.c ${APPLICATION} ${DRIVERS} ${THIRD_PARTY})

add_custom_target(${PROJECT_NAME}.bin
        COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin -Obinary
        DEPENDS ${PROJECT_NAME}.elf
        COMMENT "Preparing bin file: ${CMAKE_OBJCOPY} ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin -Obinary")

add_custom_target(send
        COMMAND /opt/arm/stm32/ocd/qfn4
        DEPENDS ${PROJECT_NAME}.bin
        COMMENT "SENDING")