
NRFLITE = ../NRFLite
AESLIB = ../AESLib

CC = avr-gcc
CXX = avr-g++
AS = avr-as
CFLAGS ?= -Wall -Werror
CXXFLAGS ?= -Wall -Werror
ASFLAGS += -DARDUINO=185 -DARDUINO_ARCH_AVR -mmcu=atmega328p -DF_CPU=16000000L
CFLAGS += -Os -ffunction-sections -fdata-sections -flto
CFLAGS += -DARDUINO=185 -DARDUINO_ARCH_AVR -mmcu=atmega328p -DF_CPU=16000000L
CXXFLAGS += -Os -ffunction-sections -fdata-sections -flto -fpermissive -fno-exceptions -fno-threadsafe-statics -fno-devirtualize
CXXFLAGS += -DARDUINO=185 -DARDUINO_ARCH_AVR -mmcu=atmega328p -DF_CPU=16000000L
CPPFLAGS += -I/usr/share/arduino/hardware/archlinux-arduino/avr/cores/arduino
CPPFLAGS += -I/usr/share/arduino/hardware/archlinux-arduino/avr/cores/arduino/api
CPPFLAGS += -I/usr/share/arduino/hardware/archlinux-arduino/avr/variants/standard
CPPFLAGS += -I/usr/share/arduino/hardware/archlinux-arduino/avr/libraries/SPI/src
CPPFLAGS += -I$(NRFLITE)/src
CPPFLAGS += -I$(AESLIB)
LDFLAGS += -Wl,--gc-sections -fuse-linker-plugin
LIBS += -lc -lm

OBJS = \
  ttypods.o \
  main.o \
  core-wiring.o \
  core-wiring_digital.o \
  core-hooks.o \
  core-Print.o \
  core-CDC.o \
  core-HardwareSerial.o \
  core-HardwareSerial0.o \
  ardlib-SPI.o \
  NRFLite.o \
  AESLib.o \
  aes_keyschedule.o \
  aes_sbox.o \
  aes_enc.o \
  aes_dec.o \
  aes_invsbox.o

ttypods.elf: $(OBJS)
	$(LINK.cpp) -o $@ $^

main.o: /usr/share/arduino/hardware/archlinux-arduino/avr/cores/arduino/main.cpp
	$(COMPILE.cpp) -o $@ $^

core-%.o: /usr/share/arduino/hardware/archlinux-arduino/avr/cores/arduino/%.cpp
	$(COMPILE.cpp) -o $@ $^

core-%.o: /usr/share/arduino/hardware/archlinux-arduino/avr/cores/arduino/%.c
	$(COMPILE.c) -o $@ $^

ardlib-SPI.o: /usr/share/arduino/hardware/archlinux-arduino/avr/libraries/SPI/src/SPI.cpp
	$(COMPILE.cpp) -o $@ $^

NRFLite.o: $(NRFLITE)/src/NRFLite.cpp
	$(COMPILE.cpp) -o $@ $^

AESLib.o: $(AESLIB)/AESLib.c
	$(COMPILE.c) -o $@ $^

aes_%.o: $(AESLIB)/aes_%-asm.S
	$(COMPILE.S) -o $@ $^

aes_dec.o: $(AESLIB)/aes_dec-asm_faster.S
	$(COMPILE.S) -o $@ $^
