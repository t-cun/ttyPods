
NRFLITE = ../NRFLite

CC = avr-gcc
CXX = avr-g++
CFLAGS ?= -Wall -Werror
CXXFLAGS ?= -Wall -Werror
CFLAGS += -Os -ffunction-sections -fdata-sections -flto
CFLAGS += -DARDUINO=185 -DARDUINO_ARCH_AVR -mmcu=atmega328p -DF_CPU=16000000L
CXXFLAGS += -Os -ffunction-sections -fdata-sections -flto -fpermissive -fno-exceptions -fno-threadsafe-statics -fno-devirtualize
CXXFLAGS += -DARDUINO=185 -DARDUINO_ARCH_AVR -mmcu=atmega328p -DF_CPU=16000000L
CPPFLAGS += -I/usr/share/arduino/hardware/arduino/avr/cores/arduino
CPPFLAGS += -I/usr/share/arduino/hardware/arduino/avr/cores/arduino/api
CPPFLAGS += -I/usr/share/arduino/hardware/arduino/avr/variants/standard
CPPFLAGS += -I/usr/share/arduino/hardware/arduino/avr/libraries/SPI/src
CPPFLAGS += -I$(NRFLITE)/src
LDFLAGS += -Wl,--gc-sections -fuse-linker-plugin
LIBS += -lc -lm

OBJS = \
  ttypods.o \
  main.o \
  core-wiring.o \
  core-wiring_digital.o \
  core-hooks.o \
  core-Print.o \
  core-Stream.o \
  core-CDC.o \
  core-HardwareSerial.o \
  core-HardwareSerial0.o \
  core-WInterrupts.o \
  ardlib-SPI.o \
  NRFLite.o

ttypods.elf: $(OBJS)
	$(LINK.cpp) -o $@ $^

main.o: /usr/share/arduino/hardware/arduino/avr/cores/arduino/main.cpp
	$(COMPILE.cpp) -o $@ $^

core-%.o: /usr/share/arduino/hardware/arduino/avr/cores/arduino/%.cpp
	$(COMPILE.cpp) -o $@ $^

core-%.o: /usr/share/arduino/hardware/arduino/avr/cores/arduino/%.c
	$(COMPILE.c) -o $@ $^

ardlib-SPI.o: /usr/share/arduino/hardware/arduino/avr/libraries/SPI/src/SPI.cpp
	$(COMPILE.cpp) -o $@ $^

NRFLite.o: $(NRFLITE)/src/NRFLite.cpp
	$(COMPILE.cpp) -o $@ $^
