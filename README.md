# Atari Breakout on Bare metal

This is a bare metal implementation of the game Atari Breakout. It is designed to run on 64-bit Raspberry Pi devices. It has tested this on the PI 02W. 

This project is built with [circle](https://github.com/rsta2/circle), a C++ bare metal environment for the Raspberry Pi.


## Build instructions

> For Macbook Users: There is no native `arm-gcc` compiler for Apple Silicon, you should try on an Intel Mac or a Linux machine. If all else fails, try to install a x86 Linux virtual machine and run the scripts there. I do not know if it will work, however.

This project was built with circle, so you will need to clone circle into the root directory of this project: `git clone https://github.com/rsta2/circle`. 

You will need the `arm-gcc` compiler. Circle does not have support for the clang compiler. 

Follow the configuration and build [instructions](https://circle-rpi.readthedocs.io/en/44.5/getting-started.html) to build the circle library. Your `Config.mk` file in the `circle/` directory should be the following.

```
PREFIX64 = aarch64-none-linux-gnu-
AARCH = 64
RASPPI = 3
```

To build circle, cd into `circle/lib` and run the following commands

```sh
$ ./makeall clean
$ ./makeall
```

Make sure the compiler prefix is what you have installed.

If circle successfully builds, you should find `libcircle.a` in `circle/lib`

After circle successfully builds, `cd` into the root directory and run `make`. You should end up with a `kernel8.img`. This can be copied into a microSD card and booted into a Raspberry Pi.

## microSD card setup

1. Ensure the microSD card is formatted as FAT32.
2. From this [url](https://github.com/raspberrypi/firmware/tree/master/boot) (https://github.com/raspberrypi/firmware/tree/master/boot), copy `start.elf`, `bootcode.bin`, and `fixup.dat` to the microSD card.

You can also setup the microSD card by just installing Raspbian on the card and then removing all files that starts with `kernel`, replacing it with the one you built here.

## Game instructions

Plug in a USB Keyboard into the Raspberry PI. You might need a microUSB to USB dongle. Ensure the Pi is connected to a monitor via HDMI (again, you might need a microHDMI to HDMI adapter). Power on the PI. If all goes well, you should see the rainbow splash screen followed by a screen that says "Breakout". It will then load immediately into the game. 

Pressing any button on the keyboard will cause the paddle to move in the opposite direction. Once 3 lives are up, the game will go to a "Game Over!" screen and it ends there. Press any key to play another game. 