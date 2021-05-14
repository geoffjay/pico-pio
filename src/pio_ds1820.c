#include "pio_ds1820.h"

// DS1820 ROM commands
#define SEARCH_ROM        0xF0
#define READ_ROM          0x33
#define MATCH_ROM         0x55
#define SKIP_ROM          0xCC

// DS1820 function commands
#define START_CONVERT     0x44  // Tells device to take a temperature reading and put it on the scratchpad
#define COPY_SCRATCHPAD   0x48  // Copy scratchpad to EEPROM
#define READ_SCRATCHPAD   0xBE  // Read from scratchpad
#define WRITE_SCRATCHPAD  0x4E  // Write to scratchpad
#define RECALL_SCRATCHPAD 0xB8  // Recall from EEPROM to scratchpad
#define READ_POWER_SUPPLY 0xB4  // Determine if device needs parasite power
#define ALARM_SEARCH      0xEC  // Query bus for devices with an alarm condition

// Scratchpad locations
#define TEMP_LSB        0
#define TEMP_MSB        1
#define HIGH_ALARM_TEMP 2
#define LOW_ALARM_TEMP  3
#define CONFIGURATION   4
#define INTERNAL_BYTE   5
#define COUNT_REMAIN    6
#define COUNT_PER_C     7
#define SCRATCHPAD_CRC  8

// DSROM FIELDS
#define DSROM_FAMILY    0
#define DSROM_CRC       7

// Device resolution
#define TEMP_9_BIT  0x1F //  9 bit
#define TEMP_10_BIT 0x3F // 10 bit
#define TEMP_11_BIT 0x5F // 11 bit
#define TEMP_12_BIT 0x7F // 12 bit

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

    pio_ds1820_write_bytes(pio, sm, (uint8_t[]){SKIP_ROM, START_CONVERT}, 2);
    sleep_ms(1000);
    pio_ds1820_write_bytes(pio, sm, (uint8_t[]){SKIP_ROM, READ_SCRATCHPAD}, 2);
    pio_ds1820_read_bytes(pio, sm, data, 9);

    crc = crc8(data, 9);
    if (crc != 0)
        return -2000;

    // get the temperature using two's complement 16-bit
    int t1 = data[TEMP_LSB];
    int t2 = data[TEMP_MSB];
    int16_t temp1 = (t2 << 8 | t1);
    volatile float temp = (float)temp1 / 16;

    return temp;
}

uint pio_ds1820_get_resolution(PIO pio, uint sm)
{
    uint8_t data[9];

    pio_ds1820_write_bytes(pio, sm, (uint8_t[]){SKIP_ROM, READ_SCRATCHPAD}, 2);
    pio_ds1820_read_bytes(pio, sm, data, 9);

    if (crc8(data, 9) != 0)
        return -2000;

    switch(data[CONFIGURATION])
    {
        case TEMP_12_BIT:
            return 12;
        case TEMP_11_BIT:
            return 11;
        case TEMP_10_BIT:
            return 10;
        case TEMP_9_BIT:
            return 9;
    }

    return 0;
}

void pio_ds1820_get_address(PIO pio, uint sm, uint8_t *address)
{
    uint8_t data[9];

    pio_ds1820_write_bytes(pio, sm, (uint8_t[]){READ_ROM}, 1);
    pio_ds1820_read_bytes(pio, sm, data, 9);

    if (crc8(data, 9) != 0)
        return -2000;

    address = data;
}
