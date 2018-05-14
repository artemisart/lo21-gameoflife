QT += widgets
QMAKE_CXXFLAGS = -std=c++11 -Wall
QMAKE_LFLAGS = -std=c++11

SOURCES += \
    main.cpp \
    automate.cpp \
    autocell.cpp \
    Rule.cpp \
    gameoflife.cpp

HEADERS += \
    automate.h \
    autocell.h \
    Grid.h \
    Automaton.h \
    History.h \
    Rule.h \
    Index.h \
    gameoflife.h
