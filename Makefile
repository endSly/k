
PROJ    := k
TARGET  := $(PROJ)
OBJS    := loader.o x86.o kernel.o arch.o

CC      := gcc
LD      := ld
AS      := nasm
 
CFLAGS  := -std=c99 -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs  -nostdinc -fno-builtin -fno-stack-protector -c
LDFLAGS := -T linker.ld
ASFLAGS := -f elf
 
.PHONY : build clean

all: image
 
build: $(TARGET).bin
 
$(TARGET).bin : $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
 
.s.o:
	$(AS)  $(ASFLAGS) $<
 
image: $(TARGET).img
 
$(TARGET).img: $(TARGET).bin
	cat grub/stage1 grub/stage2 grub/pad $(TARGET).bin > $(TARGET).img
 
 
clean : 
	@rm -fv *.img
	@rm -fv *.bin
	@rm -fv *.o
