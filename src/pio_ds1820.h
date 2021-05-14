#ifndef _PIO_DS1820_H_
#define _PIO_DS1820_H_

#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "ds1820.pio.h"

void  pio_ds1820_write_bytes     (PIO pio, uint sm, uint8_t bytes[], int len);
void  pio_ds1820_read_bytes      (PIO pio, uint sm, uint8_t bytes[], int len);
float pio_ds1820_get_temperature (PIO pio, uint sm);
uint  pio_ds1820_get_resolution  (PIO pio, uint sm);
void  pio_ds1820_get_address     (PIO pio, uint sm, uint8_t *address);

#endif
