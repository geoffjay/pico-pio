# Something Something Raspberry Pi Pico + PIO

Just a place to try out the PIO on an RP2040.

## Setup

Better information for getting started exists in the official documentation, but some notes that
were generated while getting setup can be seen [here][docs/SETUP.md].

## Develop

- build the `.uf2` with

```shell
mkdir build
cd build
cmake ..
make
```

- reset the RP2040 with `BOOTSEL` enabled and copy the `.uf2` file with

```shell
sudo mount /dev/sda1 /mnt/pico
sudo cp src/pio_ds1820_read.uf2 /mnt/pico/
```

- reset the RP2040 to run the new application
- connect the signal wire of the ds1820 to GPIO2 of the RP2040 with a 4.7k pullup resistor
- connect power and ground of the ds1820 to the appropriate pins on the Raspberry Pi
- open a terminal with `minicom -b 115200 -o -D /dev/serial0`
- it should be printing the current temperature, which might not be calibrated
