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

By default, the emulator can only run a 256 byte WOZMON rom. This rom must be supplied by the user, and will not be provided in this repository. In order to run the emulator, refer to the usage example below. 

```bash
./bin/apple1 <path to WOZMON>
```

## Future Plans

The current goal right now is to debug WOZMON before I begin testing BASIC. For example, there's currently a bug where if you type in the command to display the contents in a memory range (i.e. FF00.FFFF), only the first byte will be displayed followed by the backslash. If anyone has an idea as to what's causing this bug, please open an issue/pull request. Thank you and enjoy!  
