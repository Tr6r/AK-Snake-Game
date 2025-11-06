CFLAGS		+= -I./sources/app/objects
CPPFLAGS	+= -I./sources/app/objects

VPATH += sources/app/objects

# CPP source files

SOURCES_CPP += sources/app/objects/snake.cpp
SOURCES_CPP += sources/app/objects/game.cpp
SOURCES_CPP += sources/app/objects/apple.cpp
SOURCES_CPP += sources/app/objects/obstacle.cpp
SOURCES_CPP += sources/app/objects/goldenspiral.cpp
