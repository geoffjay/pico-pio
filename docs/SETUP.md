## Setup

### Raspberry Pi

The model used at the time of writing was 3B+ 32bit, the same instructions will probably apply to other models.

- install Raspberry Pi Imager
- choose Raspbian OS Lite with the correct architecture for your board
- write the image to a MicroSSD device
- once finished unplug and re-plugin
- create a file on the `/boot` device named `ssh` with no extension
- create a file on the `/boot` device named `wpa_supplicant.conf` with the content:

```
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
country=<Insert 2 letter ISO 3166-1 country code here>

network={
 ssid="<Name of your wireless LAN>"
 psk="<Password for your wireless LAN>"
}
```

- eject the device
- plug the disk into the Raspberry Pi
- power on the Raspberry Pi
- wait... wait... wait...
- this could take some time if it's resizing the partition for your disk
- go to the DHCP leases for your router and find the device IP address
- SSH into the Raspberry Pi using the user name `pi` and password `raspberry`
- after booting in for the first time make sure to run `raspi-config` and change the password

### Pico 2040

- solder the stupid pin headers on the stupid tiny little solder points
- log into the Raspberry Pi
- plug the Pico into the Raspberry Pi using a cable that's capable of transferring data (not just power)
- setup for development with

```shell
mkdir ~/src
cd ~/src
wget https://raw.githubusercontent.com/raspberrypi/pico-setup/master/pico_setup.sh
chmod +x pico_setup.sh
./pico_setup.sh
sudo reboot
```

- SSH back into the Raspberry Pi and `cd ~/src/pico`
- mount the Pico and copy an example program (replace `sda1` with whatever the device was given)

```shell
sudo mkdir /mnt/pico
sudo mount /dev/sda1 /mnt/pico
sudo cp pico-examples/build/blink/blink.uf2 /mnt/pico
sudo sync
sudo umount /mnt/pico
```

- at this point the LED on the Pico should be blinking

#### Hello World

- on the Raspberry Pi enable UART using `raspi-config` under the Interfacing Options sub-menu
- connect the devices

Raspberry Pi      | Raspberry Pi Pico
----------------- | -----------------
GND               | GND
GPIO15 (UART_RX0) | GPIO0 (UART0_TX)
GPIO14 (UART_TX0) | GPOI1 (UART0_RX)

- section 4.5 of the Getting Started with Raspberry Pi Pico is more useful for this than these instructions
- unplug the Pico and plug it back in with the BOOTSEL button pressed
- copy the binary for the example

```shell
sudo mount /dev/sda1 /mnt/pico
sudo cp ~/src/pico/pico-examples/build/hello_world/serial/hello_serial.uf2 /mnt/pico/
```

- start a terminal on the Raspberry Pi with `minicom -b 115200 -o -D /dev/serial0`
- it should start printing "Hello, World" to the terminal every second

### References

- https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
- https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-python-sdk.pdf
