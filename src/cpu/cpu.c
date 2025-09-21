#include "cpu.h"
#include "instruction.h"

void cpu_init(cpu_t *cpu)
{
    // Clear Memory
    memset(cpu->memory, 0, sizeof(cpu->memory));

    // Registers
    cpu->SP = 0xFF;
    cpu->A = cpu->X = cpu->Y = 0;

    // Status flags
    cpu->N = cpu->V = cpu->B = cpu->D = cpu->Z = cpu->C = false;
    cpu->I = true; // interrupts disabled on reset

    // PIA State
    cpu->key_ready = false;
    cpu->key_value = 0;

    // Interrupt Handler
    cpu->memory[INTERRUPT_HANDLER] = 0x00;
    cpu->memory[INTERRUPT_HANDLER + 1] = 0xF0;

    // Set reset vector in ROM
    cpu->memory[RESET_LOW] = 0x00;
    cpu->memory[RESET_HIGH] = 0xFF;

    cpu->memory[BRK_HANDLER] = 0x00;
    cpu->memory[BRK_HANDLER + 1] = 0x00;

    cpu->running = true;
    cpu->global_cycles = 0;
}

void cpu_cycle(cpu_t *cpu)
{
    u8 opcode_byte = read_memory(cpu, cpu->PC++);
    opcode_t opcode = opcodes[opcode_byte];
    u16 addr;

    switch(opcode.addr_mode) {
        case IMM:
            addr = imm_address(cpu);
            break;
        case ZP:
            addr = zp_address(cpu);
            break;
        case ZPX:
            addr = zpx_address(cpu);
            break;
        case ZPY:
            addr = zpy_address(cpu);
            break;
        case ABS:
            addr = abs_address(cpu);
            break;
        case ABX:
            addr = abx_address(cpu);
            break;
        case ABY:
            addr = aby_address(cpu);
            break;
        case IND:
            addr = ind_address(cpu);
            break;
        case IDX:
            addr = indx_address(cpu);
            break;
        case IDY:
            addr = indy_address(cpu);
            break;
        case IMP:
            addr = imp_address(cpu);
            break;
        case REL:
            addr = rel_address(cpu);
            break;
    }

    opcode.operation(cpu, addr);

    cpu->global_cycles += opcode.cycles;
}

u8 load_program(cpu_t *cpu, const char* rom_path)
{
    u8 status = 1;

    FILE *fptr = fopen(rom_path, "rb");
    if (fptr == NULL) return status;

    size_t bytes_read = fread(cpu->memory + 0xFF00, sizeof(u8), 0x100, fptr);
    fclose(fptr);

    // Nothing Loaded
    if (!bytes_read) return status;

    cpu->PC = (cpu->memory[RESET_HIGH] << 8) | cpu->memory[RESET_LOW];

    status = 0;
    return status;
}

u8 read_memory(cpu_t *cpu, u16 address)
{
    if (address >= 0xD010 && address <= 0xD013) {
        switch(address) {
            case 0xD010: // keyboard data
                if (cpu->key_ready) {
                    cpu->key_ready = false;      // clear ready after read
                    return cpu->key_value | NEGATIVE_FLAG;
                }
                return 0;
            case 0xD011: // keyboard status
                return cpu->key_ready ? NEGATIVE_FLAG : 0x00;
            case 0xD012: // video data
                return 0;
            case 0xD013: // video status
                return 0;
        }
    }

    return cpu->memory[address];
}

void write_memory(cpu_t *cpu, u16 address, u8 value)
{
    if (address == 0xD012) {
        u8 ch = value & 0x7F;
        if (ch == 0x7F) {
            addch('\b');
        } else if (ch == '\n' || ch == '\r') {
            addch('\n');
        } else if (ch >= 0x20 && ch <= 0x7E) {
            addch(ch);
        } 
        refresh();
        return;
    }

    cpu->memory[address] = value;
}

void poll_keyboard(cpu_t *cpu) {
    int key_hit = getch();
    if (key_hit != ERR) {
        if (key_hit == '\n') {
            key_hit = '\r';
        } else if (key_hit >= 'a' && key_hit <= 'z'){
			key_hit += 'A' - 'a';
        }

        cpu->key_value = key_hit & 0x7F;
        cpu->key_ready = true;
    }
}

void N_flag(cpu_t *cpu, u8 value)
{
    cpu->N = (value & 0x80) != 0; 
}

void V_flag(cpu_t *cpu, u8 value, u8 result)
{
    cpu->V = ((cpu->A ^ result) & (value ^ result) & NEGATIVE_FLAG) != 0;
}

void Z_flag(cpu_t *cpu, u8 value)
{
    cpu->Z = (value == 0);
}

void C_flag(cpu_t *cpu, u16 value)
{
    cpu->C = (value > 0xFF);
}

void cpu_display_registers(cpu_t *cpu) {
    u8 value = 0;

    value |= cpu->C ? CARRY_FLAG : 0;  
    value |= cpu->Z ? ZERO_FLAG : 0;  
    value |= cpu->I ? INTERRUPT_FLAG : 0;  
    value |= cpu->D ? DECIMAL_FLAG : 0;  
    value |= BREAK_FLAG;
    value |= 0x20;               
    value |= cpu->V ? OVERFLOW_FLAG : 0;  
    value |= cpu->N ? NEGATIVE_FLAG : 0;  

    printf("A: %02X, X: %02X, Y: %02X, PC: %04X, SP: %02X, SR: %02X\n",
               cpu->A, cpu->X, cpu->Y, cpu->PC, cpu->SP, value);
}

void print_memory(cpu_t *cpu, u16 start, u16 end) {
    printf("Memory dump from $%04X to $%04X:\n", start, end);
    for (u16 addr = start; addr <= end; addr++) {
        // Print 16 bytes per line
        if ((addr - start) % 16 == 0) {
            printf("\n%04X: ", addr); // Print address label at the start of each line
        }
        printf("%02X ", read_memory(cpu, addr));
    }
    printf("\n\n");
}