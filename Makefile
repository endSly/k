
PROJ    := k
TARGET  := $(PROJ)
OBJS    := kernel.o loader.o

CC      := gcc
LD      := ld
AS      := nasm
 
CFLAGS  := -Wall -Wextra -nostdlib -nostartfiles -nodefaultlibs
LDFLAGS := -T linker.ld
 
 
.PHONY : build clean
 
build: $(TARGET).bin
 
$(TARGET).bin : $(OBJS)
	$(OBJCOPY) -v -O binary $< $@
	-@gbafix $@
 
%.o : %.c
	$(CC) $< $(CFLAGS) -o $@
	
loader.o : loader.s
	$(LD) $(CFLAGS) -o $@
 
image: $(TARGET).img
 
$(TARGET).img: $(TARGET).bin
	cat grub/stage1 grub/stage2 grub/pad $(TARGET).bin > $(TARGET).img
 
 
clean : 
	@rm -fv *.img
	@rm -fv *.bin
	@rm -fv *.o
