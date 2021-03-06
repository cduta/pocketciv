#include "BuildFarmInstruction.hpp"

#include <QMessageBox>

BuildFarmInstruction::BuildFarmInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction)
{
    this->nextInstruction->setKeepInstruction(true);
}

void BuildFarmInstruction::initInstruction()
{
    this->boardModel->printMessage("BUILD FARM:");
    this->boardModel->printMessage("Choose a region without a farm and with a forest.");
    this->boardModel->printMessage("Decimate 2 tribes and the forest to build a farm.");
    this->boardModel->printMessage("Remember: At least 1 tribe has to remain anywhere in the Empire, when decimating Tribes!");
    this->boardModel->printMessage(" ");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::AGRICULTURE) && this->boardModel->hasAgricultureLeft())
    {
        this->boardModel->printMessage("AGRICULTURE: ");
        this->boardModel->printMessage("Once per round, don't decimate a Forest to create a Farm.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("When you are done, press Done.");
    this->boardModel->printMessage(" ");
    this->boardModel->disableButtons();
    this->boardModel->setDoneButton(true);
    return;
}

Instruction *BuildFarmInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(!regionModel->hasFarm() && regionModel->getTribes() >= 2 && this->boardModel->getTribeCount() > 2 && regionModel->hasForest())
        {
            regionModel->setTribes(regionModel->getTribes() - 2);
            if(this->boardModel->hasAdvanceAquired(AdvanceModel::AGRICULTURE) && this->boardModel->hasAgricultureLeft())
            {
                QMessageBox::StandardButton result =
                        QMessageBox::StandardButton(
                        QMessageBox::question(NULL,
                                             "Advance (AGRICULTURE)",
                                             "Do you want to use Advance (AGRICULTURE) to build a farm"
                                             "without decimating a Forest?\n\n"
                                              "You can use this ONCE per turn.",
                                              QMessageBox::Yes,QMessageBox::No));

                if(result == QMessageBox::Yes)
                {
                    this->boardModel->setAgricultureLeft(false);
                    this->boardModel->printMessage("Advance (AGRICULTURE) used for building a farm.");
                }
                else
                {
                    regionModel->setForest(false);
                }
            }
            else
            {
                regionModel->setForest(false);
            }

            regionModel->setFarm(true);
        }
    }

    return this;
}

Instruction *BuildFarmInstruction::triggerDone()
{
    this->boardModel->disableButtons();
    this->boardModel->enableMainPhaseButtons();

    this->boardModel->setAgricultureLeft(true);
    this->nextInstruction->setKeepInstruction(false);
    this->nextInstruction->initInstruction();
    return this->nextInstruction;
}
