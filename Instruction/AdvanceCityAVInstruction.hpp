#ifndef ADVANCECITYAVINSTRUCTION_H
#define ADVANCECITYAVINSTRUCTION_H

#include <QMessageBox>

#include <Instruction/Instruction.hpp>
#include "BoardModel.hpp"

class AdvanceCityAVInstruction : public Instruction
{
    BoardModel *boardModel;
    int availableAdvancements;
    int maximumCityAV;
    int toBePaid;
    bool capitolBeyondFour;
    bool capitolAdvanced;
    int region;
    int reduceCost;
    QMessageBox *chooseRessource;

public:
    AdvanceCityAVInstruction(BoardModel *boardModel);

    virtual void initInstruction();

    virtual Instruction *triggerHex(Qt::MouseButton button, int x, int y);
    virtual Instruction *triggerDone();

private slots:
    void ressourceChosen(QAbstractButton *button);
};

#endif // ADVANCECITYAVINSTRUCTION_H
