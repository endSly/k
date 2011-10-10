
#include "arch.h"
#include "types.h"
#include "macros.h"

#include "lib.h"

byte_t* video_buffer = (byte_t *) 0xb8000;
word_t cursor_pos = 0;

INLINE void update_cursor(void)
{
    outb(0x3D4, 14);
    outb(0x3D5, (cursor_pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_pos & 0xFF);
}

void kcls(void)
{
    for (int i = 0; i < 80 * 25; i++) {
        video_buffer[i << 1] = ' '; 
        video_buffer[(i << 1) + 1] = 0x07;
    }
    
    cursor_pos = 0;
    update_cursor();
}

int kputchar(int c)
{
    video_buffer[cursor_pos << 1] = c; 
    video_buffer[(cursor_pos << 1) + 1] = 0x07;
    
    cursor_pos++;
    update_cursor();
    return c;
}

int kputs(const char* str)
{
    int len = 0;
    for (char* s = str; *s; s++, cursor_pos++, len++) {
        video_buffer[cursor_pos << 1] = *s; 
        video_buffer[(cursor_pos << 1) + 1] = 0x07;
    }

    update_cursor();
    return len;
}


