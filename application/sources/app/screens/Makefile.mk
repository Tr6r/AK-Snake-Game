CFLAGS		+= -I./sources/app/screens
CPPFLAGS	+= -I./sources/app/screens

VPATH += sources/app/screens

# CPP source files

SOURCES_CPP += sources/app/screens/scr_startup.cpp
SOURCES_CPP += sources/app/screens/scr_menu.cpp
SOURCES_CPP += sources/app/screens/scr_gameplay.cpp
SOURCES_CPP += sources/app/screens/scr_config.cpp
SOURCES_CPP += sources/app/screens/scr_gamemap.cpp
SOURCES_CPP += sources/app/screens/scr_idle.cpp
