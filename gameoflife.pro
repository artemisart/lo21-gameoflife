QT += widgets
QMAKE_CXXFLAGS = -std=c++11 -Wall
QMAKE_LFLAGS = -std=c++11

SOURCES += \
    main.cpp \
    automate.cpp \
    autocell.cpp \
    Grid.cpp \
    Automaton.cpp \
    History.cpp \
    Rule.cpp \
    gameoflife.cpp \
    dialog.cpp \
    Automate1D.cpp

HEADERS += \
    automate.h \
    autocell.h \
    Grid.h \
    Automaton.h \
    History.h \
    Rule.h \
    Index.h \
    gameoflife.h \
    dialog.h \
    Automate1D.h

FORMS += \
    dialog.ui \
    Automate1D.ui
