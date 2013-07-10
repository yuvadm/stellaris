# Stellaris

A ready-made repository for writing, compiling and flashing code for the TI Stellaris.

Tested on Mac OS X 10.6.8, should work on most Macs and Linuxes.

## Requirements

 - [ARM EABI Toolchain Builder](https://github.com/jsnyder/arm-eabi-toolchain)
 - [lm4tools](https://github.com/utzig/lm4tools)
 - TI Stellaris LM4F120XL dev board


## Usage
 - `cd boards/ek-lm4f120xl/project0`
 - `make`
 - `lm4flash gcc/project0.bin`
 - Great success!
