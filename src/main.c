#include "cpu/cpu.h"
#include "cpu/instruction.h"

int main(int argc, char *argv[])
{
    // Initialize CPU
    u8 status;
    cpu_t cpu;
    cpu_init(&cpu);

    // Init WOZMON/Basic
    if (!init_software(&cpu)) {
        fprintf(stderr, "There was an error loading the Apple II Rom\n");
        cpu.running = false;
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