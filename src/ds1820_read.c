#include <stdio.h>
#include "pico/stdlib.h"
#include "pio_ds1820.h"

char *bytes_to_str(uint8_t buf[], uint len) {
    char str[len * 3];
    unsigned char *pin = buf;
    const char *hex = "0123456789ABCDEF";
    char *pout = str;

    for (int i = 0; i < sizeof(buf) - 1; ++i)
	{
        *pout++ = hex[(*pin>>4) & 0xF];
        *pout++ = hex[(*pin++) & 0xF];
        *pout++ = ':';
    }

    *pout++ = hex[(*pin>>4) & 0xF];
    *pout++ = hex[(*pin) & 0xF];
    *pout = 0;

	return str;
}

int main()
{
    stdio_init_all();

    float temperature;
    uint8_t address[8];

    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &ds1820_program);

    ds1820_init(pio, sm, offset, 2);

    uint resolution = pio_ds1820_get_resolution(pio, sm);
    pio_ds1820_get_address(pio, sm, address);
    // char *str_address = bytes_to_str(address, 8);

    for (;;)
    {
        do
        {
            temperature = pio_ds1820_get_temperature(pio, sm);
        } while (temperature < -999);

        printf("{\"address\":\"%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\",\"resolution\":\"%d bits\",\"temperature\":%f}\r\n",
               address[0], address[1], address[2], address[3], address[4], address[5], address[6], address[7],
               resolution, temperature);

        sleep_ms(500);
    };

    return 0;
}
