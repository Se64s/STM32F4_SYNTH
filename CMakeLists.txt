cmake_minimum_required(VERSION 3.15.3)

# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)

# Path to toolchain file. This one has to be before 'project()' below
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/arm-none-eabi-gcc.cmake)

# Setup project info
project(STM32F4_SYNTH)

# Define target name
set(EXECUTABLE ${PROJECT_NAME}.elf)

# Define languages used
enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Setup app version
set(APP_VERSION 0.0.1 CACHE INTERNAL "App version")

# Define linker file path
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/Device/STM32F446RETx_FLASH.ld)

# List of source files
set(SRC_FILES
    ${SRC_FILES}

    App/Src/main.c
    App/Src/freertos.c
    App/Src/cli_task.c
    App/Src/cli_usr_cmd.c
    App/Src/audio_task.c
    App/Src/midi_task.c

    Device/system_stm32f4xx.c

    Device/startup_stm32f446xx.s

    Lib/cbuf/Src/circular_buffer.c

    Lib/midi/Src/midi_voice.c
    Lib/midi/Src/midi_lib.c

    Lib/SWO/Src/swo.c

    Lib/printf/Src/printf.c

    Lib/Shell/Src/shell.c

    Lib/audio_lib/Src/audio_engine.c
    Lib/audio_lib/Src/audio_effect.c
    Lib/audio_lib/Src/audio_env_ramp.c
    Lib/audio_lib/Src/audio_env_adsr.c
    Lib/audio_lib/Src/audio_delay.c
    Lib/audio_lib/Src/audio_filter.c
    Lib/audio_lib/Src/audio_wavetable.c
    Lib/audio_lib/Src/audio_tools.c
    Lib/audio_lib/Src/audio_hal.c

    Sys/Sys_Common/Src/sys_common.c

    Sys/Sys_Log/Src/sys_log.c

    Sys/Sys_Error/Src/user_assert.c
    Sys/Sys_Error/Src/sys_error.c

    Sys/Sys_HAL/Src/stm32f4xx_it.c
    Sys/Sys_HAL/Src/stm32f4xx_hal_msp.c
    Sys/Sys_HAL/Src/stm32f4xx_sys_hal_mcu.c
    Sys/Sys_HAL/Src/stm32f4xx_sys_hal_gpio.c
    Sys/Sys_HAL/Src/stm32f4xx_sys_hal_dac.c
    Sys/Sys_HAL/Src/stm32f4xx_sys_hal_tim.c
    Sys/Sys_HAL/Src/stm32f4xx_sys_hal_usart.c
    Sys/Sys_HAL/Src/stm32f4xx_sys_hal_dma.c
    Sys/Sys_HAL/Src/stm32f4xx_sys_hal_adc.c
    Sys/Sys_HAL/Src/stm32f4xx_sys_hal_i2s.c

    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c

    Middlewares/Third_Party/FreeRTOS/Source/croutine.c
    Middlewares/Third_Party/FreeRTOS/Source/event_groups.c
    Middlewares/Third_Party/FreeRTOS/Source/list.c
    Middlewares/Third_Party/FreeRTOS/Source/queue.c
    Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c
    Middlewares/Third_Party/FreeRTOS/Source/tasks.c
    Middlewares/Third_Party/FreeRTOS/Source/timers.c
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
)

# Build the executable based on the source files
add_executable(${EXECUTABLE} ${SRC_FILES})

# List of compiler defines, prefix with -D compiler option
target_compile_definitions(${EXECUTABLE} PRIVATE
    -DSTM32F446xx
    -DUSE_HAL_DRIVER
    -DPRINTF_INCLUDE_CONFIG_H
    -DARM_MATH_CM4
    -D__FPU_PRESENT
)

# List of included directories
target_include_directories(${EXECUTABLE} PRIVATE
    App/Inc

    Lib/SWO/Inc
    Lib/printf/Inc
    Lib/Shell/Inc
    Lib/audio_lib/Inc
    Lib/cbuf/Inc
    Lib/midi/Inc

    Sys/Sys_Common/Inc
    Sys/Sys_Log/Inc
    Sys/Sys_HAL/Inc
    Sys/Sys_Error/Inc

    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy

    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F

    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
    Drivers/CMSIS/DSP/Include
)

# Compiler options
target_compile_options(${EXECUTABLE} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -fdata-sections
    -ffunction-sections
    -specs=nano.specs
    -Wall
    -O0
    -g3
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
    -T${LINKER_FILE}
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    # Check why get errors using nano in cmake and not in makefile
    # -specs=nano.specs
    -specs=nosys.specs

    -lc
    -lm
    -lnosys
    -L${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Lib/GCC
    -larm_cortexM4lf_math
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    -Xlinker -print-memory-usage -Xlinker
)

# Optional: Print executable size as part of the post build process
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND ${CMAKE_SIZE_UTIL} ${EXECUTABLE}
)

# Optional: Create hex, bin and S-Record files after the build
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O srec --srec-len=64 ${EXECUTABLE} ${PROJECT_NAME}.s19
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)