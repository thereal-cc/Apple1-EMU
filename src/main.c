#include "cpu/cpu.h"
#include "cpu/instruction.h"

int main(int argc, char *argv[])
{
    // Initialize CPU
    cpu_t cpu;
    cpu_init(&cpu);

    // Init WOZMON/Basic
    if (!init_software(&cpu)) {
        fprintf(stderr, "There was an error loading the Apple II Rom\n");
        cpu.running = false;
    }

    // Load User Program, if it exists
    if (argc == 3) {
        char *end;
        errno = 0;

        unsigned long parsed = strtoul(argv[2], &end, 16);

        if (errno != 0 || *end != '\0' || parsed > UINT16_MAX) {
            fprintf(stderr, "Invalid value: %s\n", argv[2]);
            return 1;
        }

        u16 start_addr = (u16)parsed;

        if (load_program(&cpu, argv[1], parsed) != 0) {
            fprintf(stderr, "Program was not loaded, booting into Wozmon...\n");
        }
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