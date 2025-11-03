#stm32 libs path

CFLAGS		+= -I./sources/platform/stm32l/arduino/cores/stm32
CPPFLAGS	+= -I./sources/platform/stm32l/arduino/cores/stm32

VPATH += sources/platform/stm32l/arduino/cores/stm32

SOURCES += sources/platform/stm32l/arduino/cores/stm32/dtostrf.c
SOURCES += sources/platform/stm32l/arduino/cores/stm32/hooks.c
