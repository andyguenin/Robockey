###############################################################################
# Makefile for the project robockey
###############################################################################

## General Flags
PROJECT = robockey
MCU = atmega32u4
TARGET = robockey.elf
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
LDFLAGS +=  -Wl,-Map=robockey.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Libraries
LIBS = -lm 

## Objects that must be built in order to link
OBJECTS = robockey.o position.o common_robockey.o communication.o demo.o goalie.o grinder.o m_bus.o m_rf.o m_usb.o m_wii.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) robockey.hex robockey.eep robockey.lss size

## Compile
robockey.o: ../robockey.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

position.o: ../position.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

common_robockey.o: ../common_robockey.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

communication.o: ../communication.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

demo.o: ../demo.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

goalie.o: ../goalie.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

grinder.o: ../grinder.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

m_bus.o: ../m_bus.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

m_rf.o: ../m_rf.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

m_usb.o: ../m_usb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

m_wii.o: ../m_wii.c
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
	-rm -rf $(OBJECTS) robockey.elf dep/* robockey.hex robockey.eep robockey.lss robockey.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

