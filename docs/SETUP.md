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

### References

- https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf
- https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-python-sdk.pdf
