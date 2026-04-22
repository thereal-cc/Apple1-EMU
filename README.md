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
Pre-compiled binaries for WOZMON & Integer Basic are included in the roms folder. In order to run the emulator, type out the following command.

```bash
./bin/apple1 'path to program you want to load' 'start address of that program (in hex)'
```

The emulator uses F1-F3 for the following functions:

- F1: Resets the Computer (same as pressing RESET on real hardware)
- F2: Clears the terminal screen
- F3: Exits the Emulator
