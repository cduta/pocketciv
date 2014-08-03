#-------------------------------------------------
#
# Project created by QtCreator 2014-07-21T22:40:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PocketCiv
TEMPLATE = app


SOURCES += main.cpp\
        PocketCivMain.cpp \
    HexItem.cpp \
    HexModel.cpp \
    BoardModel.cpp \
    Instruction/Instruction.cpp \
    Instruction/ChooseRegionInstruction.cpp \
    Common.cpp

HEADERS  += \
    PocketCivMain.hpp \
    HexItem.hpp \
    HexModel.hpp \
    BoardModel.hpp \
    Instruction/Instruction.hpp \
    Instruction/ChooseRegionInstruction.hpp \
    Common.hpp

FORMS    +=

RESOURCES += \
    PocketCiv.qrc
