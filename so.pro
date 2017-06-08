TEMPLATE = app
CONFIG += c++14 console
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lreadline

SOURCES += main.cpp \
    input.cpp \
    CommandHandler.cpp \
    ls.cpp \
    tac.cpp \
    dir.cpp

QMAKE_CXXFLAGS += -O2

HEADERS += \
    input.h \
    CommandHandler.h \
    comands.h
