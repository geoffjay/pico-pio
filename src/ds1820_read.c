#include <stdio.h>
#include "pico/stdlib.h"
#include "pio_ds1820.h"

int main()
{
    stdio_init_all();

    PIO pio = pio0;
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &ds1820_program);

    ds1820_init(pio, sm, offset, 2);

    printf("setup\n");

    float t;
    for (;;)
    {
        do
        {
            printf(".");
            t = pio_ds1820_get_temperature(pio, sm);
        } while (t < -999);

        printf("\ntemperature %f\r\n", t);
        sleep_ms(500);
    };

    return 0;
}
