QT += widgets
QMAKE_CXXFLAGS = -std=c++11 -Wall
QMAKE_LFLAGS = -std=c++11

SOURCES += \
    main.cpp \
    #automate.cpp \
    #autocell.cpp \
    Rule.cpp \
    #gameoflife.cpp \
    automate_1d.cpp \
    welcome.cpp \
    automate_2d.cpp

HEADERS += \
    #automate.h \
    #autocell.h \
    Grid.h \
    Automaton.h \
    History.h \
    Rule.h \
    Index.h \
    #gameoflife.h \
    automate_1d.h \
    welcome.h \
    automate_2d.h

FORMS += \
    automate_1d.ui \
    welcome.ui \
    automate_2d.ui
