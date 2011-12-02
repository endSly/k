// For more information http://www.jamesmolloy.co.uk/tutorial_html/3.-The%20Screen.html

#include "arch/screen.h"

#include "arch/arch.h"

// Position of the framebuffer in RAM
#define DIR_FRAMEBUFFER 0xB8000

// Screen values
#define SCREEN_CHARS_WIDE 80
#define SCREEN_CHARS_HIGH 25
#define SCREEN_SIZE       (80 * 25)
#define TAB_SIZE          8

// VGA controller values
#define VGA_CONTROL_PORT 0x3D4
#define VGA_DATA_PORT    0x3D5
#define VGA_CONTROL_HIGH 14
#define VGA_CONTROL_LOW  15

word_t* const video_buffer = (word_t*) DIR_FRAMEBUFFER;
uint16_t cursor_pos = 0;

INLINE void update_cursor(void)
{
    // Send the high cursor byte
    outb(VGA_CONTROL_PORT, VGA_CONTROL_HIGH);
    outb(VGA_DATA_PORT, cursor_pos >> 8);
    // Send the low cursor byte
    outb(VGA_CONTROL_PORT, VGA_CONTROL_LOW);
    outb(VGA_DATA_PORT, cursor_pos);
}

INLINE void scroll(void)
{
    // Move all lines one row up
    int i = SCREEN_CHARS_WIDE;
    for (; i < SCREEN_SIZE; ++i)
        video_buffer[i-SCREEN_CHARS_WIDE] = video_buffer[i];

    // Erase the last line
    for (i -= SCREEN_CHARS_WIDE; i < SCREEN_SIZE; ++i)
        video_buffer[i] = SCREEN_COLOR(BLACK, L_GREY) << 8 | ' ';

    cursor_pos -= SCREEN_CHARS_WIDE;
}

// Write a char at the cursor's current position
void arch_putc_color(uint8_t c, uint8_t color)
{
    switch (c) {
        case '\n':
            while (cursor_pos % SCREEN_CHARS_WIDE)
                video_buffer[cursor_pos++] = (color << 8) | ' ';
            break;
        case '\r':
            do {
                video_buffer[cursor_pos] = (color << 8) | ' ';
            } while (--cursor_pos % SCREEN_CHARS_WIDE);
            break;
        case '\b':
            if (cursor_pos)
                video_buffer[cursor_pos--] = (color << 8) | ' ';
            break;
        case '\t':
            do {
                video_buffer[cursor_pos++] = (color << 8) | ' ';
            } while (cursor_pos % TAB_SIZE);
            break;
        default:
            video_buffer[cursor_pos++] = (color << 8) | c;
            break;
    }

    if (cursor_pos >= SCREEN_SIZE)
        scroll();

    update_cursor();
}

// Clear the screen and set the cursor to the top-left position
void arch_cls(void)
{
    for (int i = 0; i < SCREEN_SIZE; i++)
        video_buffer[i] = SCREEN_COLOR(BLACK, L_GREY) << 8 | ' ';

    cursor_pos = 0;
    update_cursor();
}
