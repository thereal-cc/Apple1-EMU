#include "cpu/cpu.h"
#include "cpu/instruction.h"

int main(int argc, char *argv[])
{
    // Initialize CPU
    u8 status;
    cpu_t cpu;
    cpu_init(&cpu);

    // Load Wozmon
    status = load_program(&cpu, "./roms/wozmon.bin", 0xFF00);
    if (status)
    {
        fprintf(stderr, "Error: Could not load Wozmon\n");
        return EXIT_FAILURE;
    }

    // Load Basic
    status = load_program(&cpu, "./roms/a1basic.bin", 0xE000);
    if (status)
    {
        fprintf(stderr, "Error: Could not load Wozmon\n");
        return EXIT_FAILURE;
    }

    // Init Interface
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // make getch() non-blocking
    keypad(stdscr, TRUE);  // handle special keys
    scrollok(stdscr, TRUE);

    // CPU Clock Cycle
    while (cpu.running)
    {
        cpu_cycle(&cpu);
        poll_keyboard(&cpu);
    }

    endwin();
    return EXIT_SUCCESS;
}