cmake_minimum_required(VERSION 3.12)
project(DustSensor C)

set(CMAKE_C_STANDARD 11)

set(THIRD_PARTY src/startup/startup_stm32.c src/utils/delay.c src/utils/gpio.c)

add_executable(${PROJECT_NAME}.elf src/main.c ${THIRD_PARTY})

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin -Obinary
        DEPENDS ${PROJECT_NAME}.elf)

add_custom_target(send
        COMMAND /opt/arm/stm32/ocd/qfn4
        DEPENDS ${PROJECT_NAME}.elf)