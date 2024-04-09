add_executable(spi
        spi.c
        )

#target_sources(print PRIVATE spi.c)

pico_enable_stdio_usb(spi 1)
pico_enable_stdio_uart(spi 0)

# pull in common dependencies
target_link_libraries(spi pico_stdlib hardware_spi)


# create map/bin/hex file etc.
pico_add_extra_outputs(spi)

