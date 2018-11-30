set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_VERSION   1)
set(CMAKE_SYSTEM_PROCESSOR arm-eabi)
set(CMAKE_CROSSCOMPILING 1)


set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)

set(CMAKE_FLAGS)

set(CMAKE_C_FLAGS "-DSTM32F411xE \
    -mthumb -mcpu=cortex-m4 \
    -O2 -g \
    -Wall -Wextra -pedantic \
    -ffunction-sections -fdata-sections \
	-I/opt/arm/stm32/inc \
	-I/opt/arm/stm32/CMSIS/Include \
	-I/opt/arm/stm32/CMSIS/Device/ST/STM32F4xx/Include")

set(CMAKE_EXE_LINKER_FLAGS  "-mthumb -mcpu=cortex-m4 \
    -Wl,--gc-sections -nostartfiles -L/opt/arm/stm32/lds -Tstm32f411re.lds")