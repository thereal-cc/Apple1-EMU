# Apple1-EMU

A simple Apple 1 Emulator written in C. This emulator has currently been tested with WOZMON, but testing with BASIC will be coming soon.

## Building

Follow these steps in order to build the emulator (make sure ncurses is installed first):

```bash
git clone 
cd Apple1-EMU
make clean & make
```

## Usage

By default, the emulator can only run a 256 byte WOZMON rom. This rom must be supplied by the user, and will not be provided in this repository. In order to run the emulator, put wozmon.bin in the roms folder and type out the following command. 

```bash
./bin/apple1
```
