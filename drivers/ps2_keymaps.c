#include <stdint.h>
#include "ps2_keymaps.h"

uint8_t ps2_scancodes_set1[128] = {
    ERROR_KEY,    /* error */
    SPECIAL_KEY,  /* esc */
    PRINTING_KEY, /* 1 */
    PRINTING_KEY, /* 2 */
    PRINTING_KEY, /* 3 */
    PRINTING_KEY, /* 4 */
    PRINTING_KEY, /* 5 */
    PRINTING_KEY, /* 6 */
    PRINTING_KEY, /* 7 */
    PRINTING_KEY, /* 8 */
    PRINTING_KEY, /* 9 */
    PRINTING_KEY, /* 0 */
    PRINTING_KEY, /* - */
    PRINTING_KEY, /* = */
    SPECIAL_KEY,  /* backspace */
    PRINTING_KEY, /* tab */
    PRINTING_KEY, /* Q */
    PRINTING_KEY, /* W */
    PRINTING_KEY, /* E */
    PRINTING_KEY, /* R */
    PRINTING_KEY, /* T */
    PRINTING_KEY, /* Y */
    PRINTING_KEY, /* U */
    PRINTING_KEY, /* I */
    PRINTING_KEY, /* O */
    PRINTING_KEY, /* P */
    PRINTING_KEY, /* [ */
    PRINTING_KEY, /* ] */
    PRINTING_KEY, /* enter */
    SPECIAL_KEY,  /* left ctrl */
    PRINTING_KEY, /* A */
    PRINTING_KEY, /* S */
    PRINTING_KEY, /* D */
    PRINTING_KEY, /* F */
    PRINTING_KEY, /* G */
    PRINTING_KEY, /* H */
    PRINTING_KEY, /* J */
    PRINTING_KEY, /* K */
    PRINTING_KEY, /* L */
    PRINTING_KEY, /* ; */
    PRINTING_KEY, /* ' */
    PRINTING_KEY, /* ` */
    SPECIAL_KEY,  /* left shift */
    PRINTING_KEY, /* \ (backslash) */
    PRINTING_KEY, /* Z */
    PRINTING_KEY, /* X */
    PRINTING_KEY, /* C */
    PRINTING_KEY, /* V */
    PRINTING_KEY, /* B */
    PRINTING_KEY, /* N */
    PRINTING_KEY, /* M */
    PRINTING_KEY, /* , */
    PRINTING_KEY, /* . */
    PRINTING_KEY, /* / */
    SPECIAL_KEY,  /* right shift */
    PRINTING_KEY, /* keypad * */
    SPECIAL_KEY,  /* right alt */
    PRINTING_KEY, /* space */
    SPECIAL_KEY,  /* caps lock */
    SPECIAL_KEY,  /* F1 */
    SPECIAL_KEY,  /* F2 */
    SPECIAL_KEY,  /* F3 */
    SPECIAL_KEY,  /* F4 */
    SPECIAL_KEY,  /* F5 */
    SPECIAL_KEY,  /* F6 */
    SPECIAL_KEY,  /* F7 */
    SPECIAL_KEY,  /* F8 */
    SPECIAL_KEY,  /* F9 */
    SPECIAL_KEY,  /* F10 */
    SPECIAL_KEY,  /* num lock */
    SPECIAL_KEY,  /* scroll lock */
    PRINTING_KEY, /* keypad 7 */
    PRINTING_KEY, /* keypad 8 */
    PRINTING_KEY, /* keypad 9 */
    PRINTING_KEY, /* keypad - */
    PRINTING_KEY, /* keypad 4 */
    PRINTING_KEY, /* keypad 5 */
    PRINTING_KEY, /* keypad 6 */
    PRINTING_KEY, /* keypad + */
    PRINTING_KEY, /* keypad 1 */
    PRINTING_KEY, /* keypad 2 */
    PRINTING_KEY, /* keypad 3 */
    PRINTING_KEY, /* keypad 0 */
    PRINTING_KEY, /* keypad . */
    ERROR_KEY,    /* 0x54 */
    ERROR_KEY,    /* 0x55 */
    ERROR_KEY,    /* 0x56 */
    SPECIAL_KEY,  /* F11 */
    SPECIAL_KEY,  /* F12 */
    ERROR_KEY,
    ERROR_KEY,
    ERROR_KEY,
    ERROR_KEY
};

char ps2_set1_ascii[128] = {
    '\0',
    '\0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    '\0',
    '\t',
    'q',
    'w',
    'e',
    'r',
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n',
    '\0',
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',
    '\'',
    '`',
    '\0',
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',
    'm',
    ',',
    '.',
    '/',
    '\0',
    '*',
    '\0',
    ' ',
    '\0',  /* caps lock */
    '\0',  /* F1 */
    '\0',  /* F2 */
    '\0',  /* F3 */
    '\0',  /* F4 */
    '\0',  /* F5 */
    '\0',  /* F6 */
    '\0',  /* F7 */
    '\0',  /* F8 */
    '\0',  /* F9 */
    '\0',  /* F10 */
    '\0',  /* num lock */
    '\0',  /* scroll lock */
    '7',
    '8',
    '9',
    '-',
    '4',
    '5',
    '6',
    '+',
    '1',
    '2',
    '3',
    '0',
    '.',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
};
char ps2_set1_shift[128] = {
    '\0',
    '\0',
    '!',
    '@',
    '#',
    '$',
    '%',
    '^',
    '&',
    '*',
    '(',
    ')',
    '_',
    '+',
    '\0',
    '\t',
    'Q',
    'w',
    'E',
    'R',
    'T',
    'Y',
    'U',
    'I',
    'O',
    'P',
    '{',
    '}',
    '\n',
    '\0',
    'A',
    'S',
    'D',
    'F',
    'G',
    'H',
    'J',
    'K',
    'L',
    ':',
    '"',
    '~',
    '\0',
    '|',
    'Z',
    'X',
    'C',
    'V',
    'B',
    'N',
    'M',
    '<',
    '>',
    '?',
    '\0',
    '*',
    '\0',
    ' ',
    '\0',  /* caps lock */
    '\0',  /* F1 */
    '\0',  /* F2 */
    '\0',  /* F3 */
    '\0',  /* F4 */
    '\0',  /* F5 */
    '\0',  /* F6 */
    '\0',  /* F7 */
    '\0',  /* F8 */
    '\0',  /* F9 */
    '\0',  /* F10 */
    '\0',  /* num lock */
    '\0',  /* scroll lock */
    '7',
    '8',
    '9',
    '-',
    '4',
    '5',
    '6',
    '+',
    '1',
    '2',
    '3',
    '0',
    '.',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
};
