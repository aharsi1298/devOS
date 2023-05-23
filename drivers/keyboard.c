#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT 0x2A
#define RSHIFT 0x36
#define LCTRL 0x1D
#define RCTRL 0x1D
#define LALT 0x38
#define RALT 0x38

#define BACKSPACE_RELEASED 0x8E
#define ENTER_RELEASED 0x9C
#define LSHIFT_RELEASED 0xAA
#define RSHIFT_RELEASED 0xB6
#define CTRL_RELEASED 0x9D
#define ALT_RELEASED 0xB8

#define CAPS_LOCK 0x3A

extern uint8_t task_running;
extern int *drivers[];

uint8_t LShift, RShift, LCtrl, RCtrl, LAlt, RAlt;

uint8_t extended_key_interrupt;

int shift = 0;
int capitals = 0;

static char key_buffer[256];

#define SC_MAX 216
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g', 
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};
const char sc_shift_ascii[] = { '?', '?', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ':', '"', '~', '?', '|', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?' /* <- Actually a question mark */, '?', '?', '?', ' '};
const char sc_capital_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};
const char sc_shift_no_capital_ascii[] = { '?', '?', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '?', '?', 'q', 'w', 'e', 'R', 't', 'y',
        'u', 'i', 'o', 'p', '{', '}', '?', '?', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', ':', '"', '~', '?', '|', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', '<', '>', '?' /* <- Actually a question mark */, '?', '?', '?', ' '};

static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);

    if(scancode > SC_MAX) return;
    if(scancode == BACKSPACE) {
        if(key_buffer[0] == '\0') return;
        backspace(key_buffer);
        kprint_backspace();
    } else if(scancode == ENTER) {
        kprint("\n");
        append(key_buffer, '\0');
        user_input(key_buffer);
        key_buffer[0] = '\0';
    } else if(scancode == LCTRL || scancode == RCTRL) {
        //
    } else if(scancode == LSHIFT || scancode == RSHIFT) {
        if(capitals == 0) capitals = 1;
        else if(capitals == 1) capitals = 0;
        shift = 1;
    } else if(scancode == LSHIFT_RELEASED || scancode == RSHIFT_RELEASED) {
        if(capitals == 0) capitals = 1;
        else if(capitals == 1) capitals = 0;
        shift = 0;
    } else if(scancode == CAPS_LOCK) {
        if(capitals == 0) capitals = 1;
        else if(capitals == 1) capitals = 0;
    } else {
        if(scancode > 88) return;
        char letter = 0;
        if(capitals == 0) {
            if(shift == 0) letter = sc_ascii[(int)scancode];
            else if(shift == 1) letter = sc_shift_no_capital_ascii[(int)scancode];
        }
        else if(capitals == 1) {
            if(shift == 0) letter = sc_capital_ascii[(int)scancode];
            else if(shift == 1) letter = sc_shift_ascii[(int)scancode];
        }
        char str[2] = {letter, '\0'};
        append(key_buffer, letter);
        kprint(str);
    }
    /*if(task_running == 1) {
        *(drivers[KEYBOARD_DRIVER])(scancode, regs);
    }*/
    UNUSED(regs);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, (isr_t)&keyboard_callback);
}

// BTW, I copied this. No sane person will write this out when not needed
void print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            kprint("ERROR");
            break;
        case 0x1:
            kprint("ESC");
            break;
        case 0x2:
            kprint("1");
            break;
        case 0x3:
            kprint("2");
            break;
        case 0x4:
            kprint("3");
            break;
        case 0x5:
            kprint("4");
            break;
        case 0x6:
            kprint("5");
            break;
        case 0x7:
            kprint("6");
            break;
        case 0x8:
            kprint("7");
            break;
        case 0x9:
            kprint("8");
            break;
        case 0x0A:
            kprint("9");
            break;
        case 0x0B:
            kprint("0");
            break;
        case 0x0C:
            kprint("-");
            break;
        case 0x0D:
            kprint("+");
            break;
        case 0x0E:
            kprint("Backspace");
            break;
        case 0x0F:
            kprint("Tab");
            break;
        case 0x10:
            kprint("Q");
            break;
        case 0x11:
            kprint("W");
            break;
        case 0x12:
            kprint("E");
            break;
        case 0x13:
            kprint("R");
            break;
        case 0x14:
            kprint("T");
            break;
        case 0x15:
            kprint("Y");
            break;
        case 0x16:
            kprint("U");
            break;
        case 0x17:
            kprint("I");
            break;
        case 0x18:
            kprint("O");
            break;
        case 0x19:
            kprint("P");
            break;
		case 0x1A:
			kprint("[");
			break;
		case 0x1B:
			kprint("]");
			break;
		case 0x1C:
			kprint("ENTER");
			break;
		case 0x1D:
			kprint("LCtrl");
			break;
		case 0x1E:
			kprint("A");
			break;
		case 0x1F:
			kprint("S");
			break;
        case 0x20:
            kprint("D");
            break;
        case 0x21:
            kprint("F");
            break;
        case 0x22:
            kprint("G");
            break;
        case 0x23:
            kprint("H");
            break;
        case 0x24:
            kprint("J");
            break;
        case 0x25:
            kprint("K");
            break;
        case 0x26:
            kprint("L");
            break;
        case 0x27:
            kprint(";");
            break;
        case 0x28:
            kprint("'");
            break;
        case 0x29:
            kprint("`");
            break;
		case 0x2A:
			kprint("LShift");
			break;
		case 0x2B:
			kprint("\\");
			break;
		case 0x2C:
			kprint("Z");
			break;
		case 0x2D:
			kprint("X");
			break;
		case 0x2E:
			kprint("C");
			break;
		case 0x2F:
			kprint("V");
			break;
        case 0x30:
            kprint("B");
            break;
        case 0x31:
            kprint("N");
            break;
        case 0x32:
            kprint("M");
            break;
        case 0x33:
            kprint(",");
            break;
        case 0x34:
            kprint(".");
            break;
        case 0x35:
            kprint("/");
            break;
        case 0x36:
            kprint("Rshift");
            break;
        case 0x37:
            kprint("Keypad *");
            break;
        case 0x38:
            kprint("LAlt");
            break;
        case 0x39:
            kprint("Spc");
            break;
        default:
            if (scancode <= 0x7f) {
                kprint("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                kprint("key up ");
                print_letter(scancode - 0x80);
            } else kprint("Unknown key up");
            break;
    }
}