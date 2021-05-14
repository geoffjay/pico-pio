add_executable(pio_ds1820_read)

pico_generate_pio_header(pio_ds1820_read ${CMAKE_CURRENT_LIST_DIR}/ds1820.pio)

target_sources(pio_ds1820_read PRIVATE
    ds1820_read.c
    pio_ds1820.c
    pio_ds1820.h
    )

target_link_libraries(pio_ds1820_read PRIVATE pico_stdlib hardware_gpio hardware_pio)
pico_add_extra_outputs(pio_ds1820_read)

pico_pio_auto_set_url(pio_ds1820_read)
