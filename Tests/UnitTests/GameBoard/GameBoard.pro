#-------------------------------------------------
#
# Project created by QtCreator 2018-10-23T20:34:04
#
#-------------------------------------------------

QT       += testlib

QT       += gui
QT		 += widgets
QT += core widgets gui

TARGET = tst_gameboardtest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++14

TEMPLATE = app

DESTDIR = bin

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

SOURCES += \
tst_gameboardtest.cpp \
../../../UI/start_dialog.cpp \
../../../UI/player.cpp \
../../../UI/main_window.cpp \
../../../UI/hexagon.cpp \
../../../UI/gameboard.cpp \
../../../UI/wheel_item.cpp \
../../../UI/gpawn.cpp \
../../../UI/gamestate.cpp \
../../../UI/gtransport.cpp \
../../../UI/g_actor.cpp \
## Ja tassa kurssin puolen koodit
../../../GameLogic/Engine/gameexception.cpp \
../../../GameLogic/Engine/formatexception.cpp \
../../../GameLogic/Engine/illegalmoveexception.cpp \
../../../GameLogic/Engine/ioexception.cpp \
../../../GameLogic/Engine/actorfactory.cpp \
../../../GameLogic/Engine/piecefactory.cpp \
../../../GameLogic/Engine/gameengine.cpp \
../../../GameLogic/Engine/initialize.cpp \
../../../GameLogic/Engine/hex.cpp \
../../../GameLogic/Engine/pawn.cpp \
../../../GameLogic/Engine/actor.cpp \
../../../GameLogic/Engine/transport.cpp \
../../../GameLogic/Engine/transportfactory.cpp \
../../../GameLogic/Engine/shark.cpp \
../../../GameLogic/Engine/kraken.cpp \
../../../GameLogic/Engine/seamunster.cpp \
../../../GameLogic/Engine/vortex.cpp \
../../../GameLogic/Engine/dolphin.cpp \
../../../GameLogic/Engine/boat.cpp \
../../../GameLogic/Engine/wheellayoutparser.cpp \

HEADERS += \
../../../UI/gamestate.hh \
../../../UI/gameboard.hh \
../../../UI/player.hh \
../../../UI/start_dialog.hh \
../../../UI/constants.hh \
../../../UI/player_info.hh \
../../../UI/main_window.hh \
../../../UI/hexagon.hh \
../../../UI/wheel_item.hh \
../../../UI/gpawn.hh \
../../../UI/gtransport.hh \
../../../UI/g_actor.hh \
../../../GameLogic/Engine/gameexception.hh \
../../../GameLogic/Engine/formatexception.hh \
../../../GameLogic/Engine/illegalmoveexception.hh \
../../../GameLogic/Engine/ioexception.hh \
../../../GameLogic/Engine/actorfactory.hh \
../../../GameLogic/Engine/piecefactory.hh \
../../../GameLogic/Engine/cubecoordinate.hh \
../../../GameLogic/Engine/gameengine.hh \
../../../GameLogic/Engine/initialize.hh \
../../../GameLogic/Engine/hex.hh \
../../../GameLogic/Engine/pawn.hh \
../../../GameLogic/Engine/igameboard.hh \
../../../GameLogic/Engine/igamerunner.hh \
../../../GameLogic/Engine/igamestate.hh \
../../../GameLogic/Engine/iplayer.hh \
../../../GameLogic/Engine/actor.hh \
../../../GameLogic/Engine/transport.hh \
../../../GameLogic/Engine/transportfactory.hh \
../../../GameLogic/Engine/shark.hh \
../../../GameLogic/Engine/kraken.hh \
../../../GameLogic/Engine/seamunster.hh \
../../../GameLogic/Engine/vortex.hh \
../../../GameLogic/Engine/dolphin.hh \
../../../GameLogic/Engine/boat.hh \
../../../GameLogic/Engine/wheellayoutparser.hh

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../../UI \
../../../GameLogic/Engine/ \
../game_state/
DEPENDPATH  += ../../../UI \
../../../GameLogic/Engine/ \
../game_state/
