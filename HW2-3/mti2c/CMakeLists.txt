add_executable(mti2c
        mti2c.c
        )

#target_sources(print PRIVATE mti2c.c)

pico_enable_stdio_usb(mti2c 1)
pico_enable_stdio_uart(mti2c 0)

# pull in common dependencies
target_link_libraries(mti2c pico_stdlib hardware_i2c)


# create map/bin/hex file etc.
pico_add_extra_outputs(mti2c)

