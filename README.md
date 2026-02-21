# Apple1-EMU

A simple Apple 1 Emulator written in C. This emulator has currently been tested with WOZMON and BASIC

## Building

Follow these steps in order to build the emulator (make sure ncurses is installed first):

```bash
git clone 
cd Apple1-EMU
make clean & make
```

## Usage

While pre-made binaries can't be distributed, [this repository](https://github.com/jefftranter/6502/tree/master/asm) contains the source files necessary, and instructions are provided to help with compiling. In order to run the emulator, put wozmon.bin and a1basic.bin in the roms folder and type out the following command. 

```bash
./bin/apple1
```

The program can be closed at any time by typing the '|' character. 
