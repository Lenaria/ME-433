add_executable(led
        led.c
        )

#target_sources(print PRIVATE led.c)

pico_enable_stdio_usb(led 1)
pico_enable_stdio_uart(led 0)

# pull in common dependencies
target_link_libraries(led pico_stdlib hardware_i2c)


# create map/bin/hex file etc.
pico_add_extra_outputs(led)

