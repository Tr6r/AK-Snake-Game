
# header path
CPPFLAGS	+= -I./sources/platform/stm32l/arduino/cores

# sources path
VPATH += sources/platform/stm32l/arduino/cores

# cpp files
SOURCES_CPP += sources/platform/stm32l/arduino/cores/wiring_digital.cpp
SOURCES_CPP += sources/platform/stm32l/arduino/cores/wiring_shift.cpp
SOURCES_CPP += sources/platform/stm32l/arduino/cores/Print.cpp
SOURCES_CPP += sources/platform/stm32l/arduino/cores/WMath.cpp
SOURCES_CPP += sources/platform/stm32l/arduino/cores/itoa.cpp

