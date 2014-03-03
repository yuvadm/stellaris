# Stellaris

A complete repository for writing, compiling and flashing code for the TI Stellaris, along with some sample projects. Based on a completely open-source toolchain, without the need to rely on cumbersome development environments.

Tested on OS X and Linux. Should work on most GNU/Linux/POSIX/*nix environments.

## Requirements

 - [ARM EABI Toolchain Builder](https://github.com/jsnyder/arm-eabi-toolchain)
 - [lm4tools](https://github.com/utzig/lm4tools)
 - TI Stellaris LM4F120XL dev board


## Usage

```bash
$ cd boards/ek-lm4f120xl/project0
$ make
$ lm4flash gcc/project0.bin
# Great success!
```
