#include "pio_ds1820.h"

static uint8_t crc8(uint8_t *data, uint8_t len)
{
    uint8_t temp;
    uint8_t databyte;
    uint8_t crc = 0;

    for (uint8_t i = 0; i < len; i++)
    {
        databyte = data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            temp = (crc ^ databyte) & 0x01;
            crc >>= 1;
            if (temp)
                crc ^= 0x8C;
            databyte >>= 1;
        }
    }

    return crc;
}

void pio_ds1820_write_bytes(PIO pio, uint sm, uint8_t bytes[], int len)
{
    pio_sm_put_blocking(pio, sm, 250);
    pio_sm_put_blocking(pio, sm, len - 1);

    for (int i = 0; i < len; i++)
    {
        pio_sm_put_blocking(pio, sm, bytes[i]);
    }
}

void pio_ds1820_read_bytes(PIO pio, uint sm, uint8_t bytes[], int len)
{
    pio_sm_put_blocking(pio, sm, 0);
    pio_sm_put_blocking(pio, sm, len - 1);

    for (int i = 0; i < len; i++)
    {
        bytes[i] = pio_sm_get_blocking(pio, sm) >> 24;
    }
}

float pio_ds1820_get_temperature(PIO pio, uint sm)
{
    uint8_t data[9];
    uint8_t crc;

    pio_ds1820_write_bytes(pio, sm, (uint8_t[]){0xCC, 0x44}, 2);
    sleep_ms(1000);
    pio_ds1820_write_bytes(pio, sm, (uint8_t[]){0xCC, 0xBE}, 2);
    pio_ds1820_read_bytes(pio, sm, data, 9);

    crc = crc8(data, 9);
    if (crc != 0)
        return -2000;

    int t1 = data[0];
    int t2 = data[1];
    int16_t temp1 = (t2 << 8 | t1);
    volatile float temp = (float)temp1 / 16;

    return temp;
}
