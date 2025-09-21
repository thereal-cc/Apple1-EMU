#include "cpu/cpu.h"
#include "cpu/instruction.h"

int main(int argc, char *argv[]) {
    cpu_t cpu;
    cpu_init(&cpu);

    char *rompath;
    if (argc != 2) {
        fprintf(stderr, "No File Loaded\n");
        return EXIT_FAILURE;
    } else {
        rompath = argv[1];
    }

    u8 status = load_program(&cpu, rompath);
    if (status) {
        fprintf(stderr, "Error: Could not load ROM\n");
        return EXIT_FAILURE;
    }

    // Init Interface
    initscr();
	cbreak();
	noecho();
    nodelay(stdscr, TRUE);  // make getch() non-blocking
    keypad(stdscr, TRUE);   // handle special keys
	

    // CPU Clock Cycle
    while (cpu.running) {
        // cpu_display_registers(&cpu);
        cpu_cycle(&cpu);
        poll_keyboard(&cpu);

        usleep(1000);
    }

    return EXIT_SUCCESS;
}