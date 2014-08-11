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
    Common.cpp \
    Instruction/ChooseFrontierInstruction.cpp \
    RegionModel.cpp \
    Instruction/InitialForestInstruction.cpp \
    Instruction/InitialMountainInstruction.cpp \
    Instruction/SetInitialDesertInstruction.cpp \
    Instruction/PlaceInitialTribes.cpp \
    EventCard.cpp \
    Instruction/MoveTribesInstruction.cpp \
    HowManyDialog.cpp \
    Instruction/MainPhaseInstruction.cpp \
    Event/Event.cpp \
    Event/EpidemicEvent.cpp \
    Instruction/DialogInstruction.cpp \
    Instruction/EndOfEraInstruction.cpp \
    Instruction/Event/EpidemicEventInstruction.cpp \
    Instruction/UpkeepInstruction.cpp \
    Instruction/PopulationGrowthInstruction.cpp

HEADERS  += \
    PocketCivMain.hpp \
    HexItem.hpp \
    HexModel.hpp \
    BoardModel.hpp \
    Instruction/Instruction.hpp \
    Instruction/ChooseRegionInstruction.hpp \
    Common.hpp \
    Instruction/ChooseFrontierInstruction.hpp \
    RegionModel.hpp \
    Instruction/InitialForestInstruction.hpp \
    Instruction/InitialMountainInstruction.hpp \
    Instruction/SetInitialDesertInstruction.hpp \
    Instruction/PlaceInitialTribes.hpp \
    EventCard.hpp \
    Instruction/MoveTribesInstruction.hpp \
    HowManyDialog.hpp \
    Instruction/MainPhaseInstruction.hpp \
    Event/Event.hpp \
    Event/EpidemicEvent.hpp \
    Instruction/DialogInstruction.hpp \
    Instruction/EndOfEraInstruction.hpp \
    Instruction/Event/EpidemicEventInstruction.hpp \
    Instruction/UpkeepInstruction.hpp \
    Instruction/PopulationGrowthInstruction.hpp

FORMS    +=

RESOURCES += \
    PocketCiv.qrc
