###############################################################################
# Makefile for the project Robockey
###############################################################################

## General Flags
PROJECT = Robockey
MCU = atmega32u4
TARGET = Robockey.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99   -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=Robockey.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = Robockey.o common_robockey.o communication.o m_bus.o m_rf.o m_wii.o m_usb.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Robockey.hex Robockey.eep Robockey.lss size

## Compile
Robockey.o: ../Robockey.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

common_robockey.o: ../common_robockey.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

communication.o: ../communication.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

m_bus.o: ../m_bus.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

m_rf.o: ../m_rf.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

m_wii.o: ../m_wii.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

m_usb.o: ../m_usb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Robockey.elf dep/* Robockey.hex Robockey.eep Robockey.lss Robockey.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

