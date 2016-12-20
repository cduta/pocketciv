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
    Instruction/PopulationGrowthInstruction.cpp \
    DecisionDialog.cpp \
    Instruction/EndGameInstruction.cpp \
    Event/AnarchyEvent.cpp \
    Instruction/Event/AnarchyEventInstruction.cpp \
    Event/BanditsEvent.cpp \
    Instruction/Event/BanditsEventInstruction.cpp \
    Instruction/AttackInstruction.cpp \
    Instruction/Event/CivilWarEventInstruction.cpp \
    Instruction/Event/EventInstruction.cpp \
    Event/CivilWarEvent.cpp \
    Event/CorruptionEvent.cpp \
    Instruction/Event/CorruptionEventInstruction.cpp \
    Event/EarthquakeEvent.cpp \
    Instruction/Event/EarthquakeEventInstruction.cpp \
    Event/FamineEvent.cpp \
    Instruction/Event/FamineEventInstruction.cpp \
    Event/FloodEvent.cpp \
    Instruction/Event/FloodEventInstruction.cpp \
    Instruction/TsunamiInstruction.cpp \
    Event/SandstormEvent.cpp \
    Instruction/Event/SandstormEventInstruction.cpp \
    Event/SuperstitionEvent.cpp \
    Instruction/Event/SuperstitionEventInstruction.cpp \
    Event/TribalWarEvent.cpp \
    Instruction/Event/TribalWarEventInstruction.cpp \
    Event/UprisingEvent.cpp \
    Instruction/Event/UprisingEventInstruction.cpp \
    Event/VolcanoEvent.cpp \
    Instruction/Event/VolcanoEventInstruction.cpp \
    Event/VisitationEvent.cpp \
    Instruction/Event/VisitationEventInstruction.cpp \
    Instruction/TradeInstruction.cpp \
    Instruction/BuildCityInstruction.cpp \
    Instruction/BuildFarmInstruction.cpp \
    Instruction/ExpeditionInstruction.cpp \
    OverviewDialog.cpp \
    AdvancesDialog.cpp \
    AdvanceItem.cpp \
    AdvanceModel.cpp \
    Instruction/AquireAdvanceInstruction.cpp \
    Instruction/CollectTaxesInstruction.cpp \
    Instruction/DecimateGoldInstruction.cpp \
    Instruction/AdvanceCityAVInstruction.cpp \
    Instruction/ReduceCityAVInstruction.cpp \
    Instruction/AssignCapitolInstruction.cpp \
    Instruction/StealingInstruction.cpp \
    DiplomaticOfferDialog.cpp

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
    Instruction/PopulationGrowthInstruction.hpp \
    DecisionDialog.hpp \
    Instruction/EndGameInstruction.hpp \
    Event/AnarchyEvent.hpp \
    Instruction/Event/AnarchyEventInstruction.hpp \
    Event/BanditsEvent.hpp \
    Instruction/Event/BanditsEventInstruction.hpp \
    Instruction/AttackInstruction.hpp \
    Instruction/Event/CivilWarEventInstruction.hpp \
    Instruction/Event/EventInstruction.hpp \
    Event/CivilWarEvent.hpp \
    Event/CorruptionEvent.hpp \
    Instruction/Event/CorruptionEventInstruction.hpp \
    Event/EarthquakeEvent.hpp \
    Instruction/Event/EarthquakeEventInstruction.hpp \
    Event/FamineEvent.hpp \
    Instruction/Event/FamineEventInstruction.hpp \
    Event/FloodEvent.hpp \
    Instruction/Event/FloodEventInstruction.hpp \
    Instruction/TsunamiInstruction.hpp \
    Event/SandstormEvent.hpp \
    Instruction/Event/SandstormEventInstruction.hpp \
    Event/SuperstitionEvent.hpp \
    Instruction/Event/SuperstitionEventInstruction.hpp \
    Event/TribalWarEvent.hpp \
    Instruction/Event/TribalWarEventInstruction.hpp \
    Event/UprisingEvent.hpp \
    Instruction/Event/UprisingEventInstruction.hpp \
    Event/VolcanoEvent.hpp \
    Instruction/Event/VolcanoEventInstruction.hpp \
    Event/VisitationEvent.hpp \
    Instruction/Event/VisitationEventInstruction.hpp \
    Instruction/TradeInstruction.hpp \
    Instruction/BuildCityInstruction.hpp \
    Instruction/BuildFarmInstruction.hpp \
    Instruction/ExpeditionInstruction.hpp \
    OverviewDialog.hpp \
    AdvancesDialog.hpp \
    AdvanceItem.hpp \
    AdvanceModel.hpp \
    Instruction/AquireAdvanceInstruction.hpp \
    Instruction/CollectTaxesInstruction.hpp \
    Instruction/DecimateGoldInstruction.hpp \
    Instruction/AdvanceCityAVInstruction.hpp \
    Instruction/ReduceCityAVInstruction.hpp \
    Instruction/AssignCapitolInstruction.hpp \
    Instruction/StealingInstruction.hpp \
    DiplomaticOfferDialog.hpp

FORMS    +=

RESOURCES += \
    PocketCiv.qrc
