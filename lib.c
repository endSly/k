
#include "lib.h"

#include "arch.h"
#include "types.h"
#include "macros.h"

#define GREY_ON_BLACK 0x07

byte_t* video_buffer = (byte_t *) 0xb8000;
word_t cursor_pos = 0;

INLINE void update_cursor(void)
{
    outb(0x3D4, 14);
    outb(0x3D5, (cursor_pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_pos & 0xFF);
}

// Clear the screen and set the cursor to the top-left position
void kcls(void)
{
    int i = 0, screen_size = 80 * 25 * 2;
    while (i < screen_size) {
        video_buffer[i++] = ' ';
        video_buffer[i++] = GREY_ON_BLACK;
    }

    cursor_pos = 0;
    update_cursor();
}

// Write a char at the current cursor position
int kputchar(const char c)
{
    video_buffer[cursor_pos << 1] = c;
    video_buffer[(cursor_pos << 1) + 1] = GREY_ON_BLACK;

    ++cursor_pos;
    update_cursor();
    return c;
}

// Write a string at the current cursor position
int kputs(const char* str)
{
    int len = 0;
    for (const char* s = str; *s; ++s, ++cursor_pos, ++len) {
        video_buffer[cursor_pos << 1] = *s;
        video_buffer[(cursor_pos << 1) + 1] = GREY_ON_BLACK;
    }

    update_cursor();
    return len;
}
