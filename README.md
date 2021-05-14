# Something Something Raspberry Pi Pico + PIO

Just a place to try out the PIO on a Pico.

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

- the RP2040 should reset and run the new application

## Other Pages

* [Setup][docs/SETUP.md]
