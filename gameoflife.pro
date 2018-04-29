QT += widgets
QMAKE_CXXFLAGS = -std=c++11
QMAKE_LFLAGS = -std=c++11

SOURCES += \
    main.cpp \
    automate.cpp \
    autocell.cpp \
    Grid.cpp \
    Automaton.cpp \
    History.cpp \
    Rule.cpp

HEADERS += \
    automate.h \
    autocell.h \
    Grid.h \
    Automaton.h \
    History.h \
    Rule.h \
    Index.h
