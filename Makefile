
PROJ    := k
TARGET  := $(PROJ)
OBJS    := kernel.o loader.o

CC      := gcc
LD      := ld
AS      := nasm
 
CFLAGS  := -std=c99 -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs
LDFLAGS := -T linker.ld
ASFLAGS := -f elf
 
.PHONY : build clean

all: image
 
build: $(TARGET).bin
 
$(TARGET).bin : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
 
kernel.o : kernel.c
	$(CC) $< $(CFLAGS) -o $@
	
loader.o : loader.s
	$(AS)  $(ASFLAGS) -o $@ $<
 
image: $(TARGET).img
 
$(TARGET).img: $(TARGET).bin
	cat grub/stage1 grub/stage2 grub/pad $(TARGET).bin > $(TARGET).img
 
 
clean : 
	@rm -fv *.img
	@rm -fv *.bin
	@rm -fv *.o
