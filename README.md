Some various TI Stellaris code I use with the open ARM EABI toolchain and lm4tools.

Tested on Mac OS X 10.6.8

Requirements
------------
 - [ARM EABI Toolchain Builder](https://github.com/jsnyder/arm-eabi-toolchain)
 - [lm4tools](https://github.com/utzig/lm4tools)
 - TI Stellaris LM4F120XL dev board


Usage
-----
 - `cd project0`
 - `make`
 - `lm4flash gcc/project0.bin`
 - Great success!
