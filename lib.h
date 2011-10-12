
#ifndef __LIB_H__
#define __LIB_H__

// Clear the screen and set the cursor to the top-left position
void kcls(void);

// Write a char at the current cursor position
int kputchar(const char c);
// Write a string at the current cursor position
int kputs(const char* str);

#endif // __LIB_H__
