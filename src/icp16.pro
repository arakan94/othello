# QT project file
# Author: Roman Ondraszek <xondra43>

TEMPLATE = app
TARGET = hra2016

CONFIG += c++11

QT = core gui widgets

DESTDIR = ../

HEADERS += \
    action.h \
    ai.h \
    ai_simple.h \
    button.h \
    command.h \
    field.h \
    game.h \
    gui_field.h \
    history.h \
    lib.h \
    othelier.h \
    player.h \
    ui_gui.h \
    ai_easy.h

SOURCES += \
    action.cc \
    ai_simple.cc \
    button.cc \
    command.cc \
    field.cc \
    game.cc \
    gui_field.cc \
    history.cc \
    hra2016.cc \
    othelier.cc \
    player.cc \
    ui_gui.cc \
    ai_easy.cc
