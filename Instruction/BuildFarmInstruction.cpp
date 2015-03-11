#include "BuildFarmInstruction.hpp"

#include <QMessageBox>

BuildFarmInstruction::BuildFarmInstruction(BoardModel *boardModel, Instruction *nextInstruction)
    : Instruction(), boardModel(boardModel), nextInstruction(nextInstruction)
{
    this->nextInstruction->setKeepInstruction(true);
}

void BuildFarmInstruction::initInstruction()
{
    this->boardModel->sendMessage("BUILD FARM:");
    this->boardModel->sendMessage("Choose a region without a farm and with a forest.");
    this->boardModel->sendMessage("Decimate 2 tribes and the forest to build a farm.");
    this->boardModel->sendMessage("At least 1 tribe has to remain in the region.");

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::AGRICULTURE) && this->boardModel->hasAgricultureLeft())
    {
        this->boardModel->sendMessage(" ");
        this->boardModel->sendMessage("AGRICULTURE: ");
        this->boardModel->sendMessage("Once per round, don't decimate a Forest to create a Farm.");
    }

    this->boardModel->sendMessage(" ");
    this->boardModel->sendMessage("When you are done, press Done.");
    this->boardModel->sendMessage(" ");
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
        if(!regionModel->hasFarm() && regionModel->getAvailableTribes() >= 2 && regionModel->hasForest())
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
                    this->boardModel->sendMessage("Advance (AGRICULTURE) used for building a farm.");
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
