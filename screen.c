
#include "screen.h"

#include "arch.h"
#include "types.h"
#include "macros.h"

#define SCREEN_SIZE (80 * 25)

#define WHITE ((0x07 << 8) + ' ')

word_t* const video_buffer = (word_t *) 0xb8000;
word_t cursor_pos = 0;

INLINE void update_cursor(void)
{
    outb(0x3D4, 14);
    outb(0x3D5, (cursor_pos >> 8) & 0xFF);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_pos & 0xFF);
}

INLINE void scroll(void)
{
    int i;
    for (i = 0; i < SCREEN_SIZE - 80; i++)
        video_buffer[i] = video_buffer[i + 80];
    
    for (; i < SCREEN_SIZE; i++)
        video_buffer[i] = WHITE;
    
    cursor_pos -= 80;
}

void arch_putc(uint8_t c)
{
    switch (c) {
        case '\n':
            while (++cursor_pos % 80)
                video_buffer[cursor_pos] = WHITE;
            break;
            
        case '\r':
            do {
                video_buffer[cursor_pos] = WHITE;
            } while (--cursor_pos % 80);
            break;
            
        case '\b':
            if (cursor_pos)
                video_buffer[cursor_pos--] = WHITE;
            break;
            
        case '\t':
            while (++cursor_pos % 8)
                video_buffer[cursor_pos] = WHITE;
            break;
            
        default:
            video_buffer[cursor_pos++] = (0x07 << 8) | c;
            break;
    }
    
    if (cursor_pos >= SCREEN_SIZE)
        scroll();
    
    update_cursor();
}

void arch_putc_color(uint8_t c, uint8_t color)
{
    switch (c) {
        case '\n':
        case '\r':
        case '\b':
        case '\t':
            arch_putc(c);
            break;
            
        default:
            video_buffer[cursor_pos] = c << 8 + color;
            if (cursor_pos >= SCREEN_SIZE)
                scroll();
            
            update_cursor();
            break;
    }
}

void arch_cls(void)
{
    for (int i = 0; i < SCREEN_SIZE; i++)
        video_buffer[i] = WHITE;
    
    cursor_pos = 0;
    update_cursor();
}
