#include "AdvanceCityAVInstruction.hpp"

#include "Instruction/ReduceCityAVInstruction.hpp"

#include <QAbstractButton>

AdvanceCityAVInstruction::AdvanceCityAVInstruction(BoardModel *boardModel)
    : boardModel(boardModel), availableAdvancements(0), capitolBeyondFour(false), capitolAdvanced(false)
{
    this->reduceCost = 0;
}

void AdvanceCityAVInstruction::initInstruction()
{
    QList<QString> advances;

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CIVIL_SERVICE))
    {
        advances.append("Advance (CIVIL SERVICE)");
        this->availableAdvancements++;
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MASONRY))
    {
        advances.append("Advance (MASONRY)");
        this->availableAdvancements++;
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::SLAVE_LABOR))
    {
        advances.append("Advance(SLAVE LABOR)");
        this->availableAdvancements++;
    }

    QString advanceString = "";

    if(advances.count() == 1)
    {
        advanceString = advances[0];
    }

    if(advances.count() == 2)
    {
        advanceString = advances[0] + " and " + advances[1];
    }

    if(advances.count() == 3)
    {
        advanceString = advances[0] + ", " + advances[1] + " and " + advances[2];
    }

    QString availableString = QString("one City AV");

    if(this->availableAdvancements > 1)
    {
        availableString = QString("%1 City AVs").arg(this->availableAdvancements);
    }

    this->boardModel->printMessage(advanceString + QString(":"));
    this->boardModel->printMessage(QString("You may advance any city's AV in the empire by up to %1").arg(availableString));
    this->boardModel->printMessage("this turn due to the advances listed above (one for each advance).");
    this->boardModel->printMessage("Remember: At least 1 tribe has to remain anywhere in the Empire, when decimating Tribes!");
    this->boardModel->printMessage(" ");

    QList<QString> advanceList;

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::BASIC_TOOLS))
    {
        advanceList.append("Advance (BASIC TOOLS)");
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::MACHINING))
    {
        advanceList.append("Advance (MACHINING)");
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::SIMPLE_TOOLS))
    {
        advanceList.append("Advance (SIMPLE TOOLS)");
    }

    QString reduceAdvances = QString("");

    if(advanceList.count() == 1)
    {
        reduceAdvances = advanceList.first();
    }
    else if(advanceList.count() > 1)
    {
        for(int i = 0; i < advanceList.count() - 2; ++i)
        {
            reduceAdvances.append(advanceList[i] + QString(", "));
        }

        reduceAdvances.append(advanceList[advanceList.count() - 2] + QString(" and ") + advanceList.last());
    }

    this->reduceCost = advanceList.count();

    if(this->reduceCost > 0)
    {
        QString reduceCostString = "one less tribe";
        this->boardModel->printMessage(reduceAdvances + QString(":"));

        if(this->reduceCost > 1)
        {
            reduceCostString = QString("%1 less tribes").arg(this->reduceCost);
        }

        this->boardModel->printMessage(QString("Increasing the City AV costs %1 (one for each advance listed above).").arg(reduceCostString));
        this->boardModel->printMessage(" ");
    }

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::ARCHITECTURE))
    {
        this->maximumCityAV = 4;
        this->boardModel->printMessage("Advance (AGRICULTURE):");
        this->boardModel->printMessage("The maximum City AV is 4.");
        this->boardModel->printMessage(" ");
    }

    if(!this->boardModel->hasAdvanceAquired(AdvanceModel::ARCHITECTURE) &&
       (this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING) ||
        this->boardModel->hasAdvanceAquired(AdvanceModel::METAL_WORKING)))
    {
        if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING) &&
           this->boardModel->hasAdvanceAquired(AdvanceModel::METAL_WORKING))
        {
            this->boardModel->printMessage("Advance (ENGINEERING) / (METAL WORKING):");
        }
        else if(this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING))
        {
            this->boardModel->printMessage("Advance (ENGINEERING):");
        }
        else if(this->boardModel->hasAdvanceAquired(AdvanceModel::METAL_WORKING))
        {
            this->boardModel->printMessage("Advance (METAL WORKING):");
        }

        this->boardModel->printMessage("The maximum City AV is 3.");
        this->boardModel->printMessage(" ");
        this->maximumCityAV = 3;
    }

    if(!this->boardModel->hasAdvanceAquired(AdvanceModel::ARCHITECTURE) &&
       !this->boardModel->hasAdvanceAquired(AdvanceModel::ENGINEERING) &&
       !this->boardModel->hasAdvanceAquired(AdvanceModel::METAL_WORKING))
    {
        this->maximumCityAV = 2;
        this->boardModel->printMessage("The maximum City AV is 2.");
        this->boardModel->printMessage(" ");
    }

    this->boardModel->printMessage("Press Done to Continue...");
}

Instruction *AdvanceCityAVInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL || this->availableAdvancements == 0)
    {
        return this;
    }

    RegionModel *activeRegion = this->boardModel->refActiveRegion();

    if(button == Qt::LeftButton)
    {
        if(activeRegion == NULL)
        {
            if(regionModel->hasCity())
            {
                this->toBePaid = regionModel->getCityAV()+1;

                if(this->boardModel->getTribeCount() <= this->toBePaid)
                {
                    this->boardModel->printMessage(QString("The amount of Tribes in the Empire is not enough to advance the City AV in Region %1.")
                                                  .arg(regionModel->getRegion()));
                    this->boardModel->printMessage(QString("To advance the City AV you'll have to pay %1 Tribes and still have at least 1 Tribe left anywhere in the Empire.")
                                                  .arg(this->toBePaid));
                    this->boardModel->printMessage(" ");
                    return this;
                }

                if(regionModel->isCapitolRegion() && regionModel->getCityAV() >= 4)
                {
                    if(this->capitolAdvanced)
                    {
                        this->boardModel->printMessage("You already advanced the capital beyond a City AV of 4 once this turn.");
                        this->boardModel->printMessage(" ");
                        return this;
                    }

                    if(regionModel->getCityAV() < 10)
                    {
                        this->boardModel->printMessage(QString("You are trying to advance the Capitol City in Region %1 beyond a City AV of 4.")
                                                      .arg(regionModel->getRegion()));
                        this->boardModel->printMessage("(BASIC TOOLS, SIMPLE TOOLS and MACHINING do NOT apply here to reduce the cost.)");
                        this->boardModel->printMessage(" ");

                        if(this->toBePaid <= activeRegion->getTribes() && this->toBePaid < this->boardModel->getTribeCount())
                        {
                            this->boardModel->printMessage(QString("To advance the Capitol City you'll have to decimate %1 Tribes from the City Region and\n"
                                                                  "a Forest, Mountain or Farm from anywhere in the Empire.")
                                                          .arg(this->toBePaid));
                            this->boardModel->printMessage(" ");
                            this->boardModel->printMessage("Select a region in which to decimate the Forest, Mountain or Farm to Advance the City AV.");
                            this->boardModel->printMessage(" ");

                            this->capitolBeyondFour = true;

                            this->boardModel->setActiveRegion(regionModel->getRegion(), false);
                            this->boardModel->setDoneText("Cancel");
                        }
                        else if(this->toBePaid <= activeRegion->getTribes() && this->toBePaid >= this->boardModel->getTribeCount())
                        {
                            this->boardModel->printMessage(QString("But, it is not possible to advance the Capitol City,"));
                            this->boardModel->printMessage(QString("because there would be no Tribes left in the Empire when decimating %1 Tribes.")
                                                          .arg(this->toBePaid));
                        }
                        else
                        {
                            this->boardModel->printMessage(QString("But, it is not possible to advance the Capitol City,"));
                            this->boardModel->printMessage(QString("because there are not enough Tribes in Region %1 to decimate %2 Tribes.")
                                                          .arg(regionModel->getRegion())
                                                          .arg(this->toBePaid));
                        }

                        this->boardModel->printMessage(" ");
                    }
                    else
                    {
                        this->boardModel->printMessage(QString("You can't advance the City AV of the Capitol in Region %1 any further than %2.")
                                                      .arg(regionModel->getRegion())
                                                      .arg(10));
                        this->boardModel->printMessage(" ");
                    }
                }
                else
                {
                    if(regionModel->getCityAV() < this->maximumCityAV || regionModel->isCapitolRegion())
                    {
                        this->toBePaid -= this->reduceCost;

                        if(this->toBePaid < 0)
                        {
                            this->toBePaid = 0;
                        }

                        this->boardModel->printMessage(QString("The cost of advancing the City AV in Region %1 is %2 Tribes from anywhere in the Empire.")
                                                      .arg(regionModel->getRegion())
                                                      .arg(this->toBePaid));
                        this->boardModel->printMessage(" ");

                        this->boardModel->setActiveRegion(regionModel->getRegion(), false);
                        this->boardModel->setDoneText("Cancel");
                    }
                    else
                    {
                        this->boardModel->printMessage(QString("You can't advance the City AV in Region %1 any further than %2.")
                                                      .arg(regionModel->getRegion())
                                                      .arg(this->maximumCityAV));
                        this->boardModel->printMessage(" ");
                    }
                }
            }
            else
            {
                this->boardModel->printMessage(QString("There is no City in Region %1.")
                                              .arg(regionModel->getRegion()));
                this->boardModel->printMessage(" ");
            }
        }
        else if(activeRegion != NULL)
        {
            if(this->capitolBeyondFour)
            {
                if(!regionModel->hasForest() && !regionModel->hasMountain() && !regionModel->hasFarm())
                {
                    this->boardModel->printMessage("This Region has none of the needed Ressources to advance the City AV of the Capitol.");
                    return this;
                }

                this->chooseRessource = new QMessageBox(QMessageBox::Question, "Choose a Ressource...", "Choose a Ressource to decimate for advancing the City AV of the Capitol...");

                if(regionModel->hasForest())
                {
                    this->chooseRessource->addButton("Forest", QMessageBox::YesRole);
                }

                if(regionModel->hasMountain())
                {
                    this->chooseRessource->addButton("Mountain", QMessageBox::NoRole);
                }

                if(regionModel->hasFarm())
                {
                    this->chooseRessource->addButton("Farm", QMessageBox::ApplyRole);
                }

                this->chooseRessource->addButton("Cancel", QMessageBox::ResetRole);

                connect(this->chooseRessource, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(ressourceChosen(QAbstractButton *)));

                this->region = regionModel->getRegion();
                this->boardModel->setDoneButton(false);
                this->chooseRessource->show();
            }
            else
            {
                if((regionModel->getTribes() - regionModel->getSelectedTribes()) > 0)
                {
                    int selectedTribes = this->boardModel->getAllSelectedTribes();

                    if(selectedTribes < this->toBePaid)
                    {
                        regionModel->setSelectedTribes(regionModel->getSelectedTribes() + 1);
                        selectedTribes++;
                    }

                    if(this->toBePaid == selectedTribes)
                    {
                        this->boardModel->printMessage(QString("Press Advance to advance the City in Region %1 to a City AV of %2.\n")
                                                      .arg(activeRegion->getRegion())
                                                      .arg(activeRegion->getCityAV()+1));
                        this->boardModel->setDoneText("Advance");
                    }
                    else
                    {
                        this->boardModel->printMessage(QString("%1/%2 tribes selected.\n")
                                                      .arg(selectedTribes)
                                                      .arg(toBePaid)
                                                      .arg(activeRegion->getRegion()));
                    }
                }
            }
        }
    }
    else if(button == Qt::RightButton)
    {
        if(activeRegion != NULL)
        {
            int selectedTribes = this->boardModel->getAllSelectedTribes();

            if(selectedTribes > 0 && regionModel->getSelectedTribes() > 0)
            {
                regionModel->setSelectedTribes(regionModel->getSelectedTribes() - 1);
                selectedTribes--;

                this->boardModel->printMessage(QString("%1/%2 tribes selected.\n")
                                              .arg(selectedTribes)
                                              .arg(toBePaid)
                                              .arg(activeRegion->getRegion()));
                this->boardModel->setDoneText("Done");
            }
        }
    }

    return this;
}

Instruction *AdvanceCityAVInstruction::triggerDone()
{
    if(this->boardModel->refActiveRegion() != NULL && this->availableAdvancements > 0)
    {
        RegionModel *activeRegion = this->boardModel->refActiveRegion();
        int selectedTribes = this->boardModel->getAllSelectedTribes();

        if(selectedTribes == this->toBePaid && !this->capitolBeyondFour)
        {
            activeRegion->increaseCityAV(1);
            this->boardModel->printMessage(QString("The City AV in region %1 has been advanced to %2.\n")
                                          .arg(activeRegion->getRegion())
                                          .arg(activeRegion->getCityAV()));
            this->boardModel->decimateAllSelectedTribes();
            this->availableAdvancements--;

            if(this->availableAdvancements == 0)
            {
                this->boardModel->printMessage("You can't increase any City AV anymore this turn.");
                this->boardModel->printMessage("Press Done to Continue...");
                this->boardModel->printMessage(" ");
                this->boardModel->unsetActiveRegion();
                this->boardModel->setDoneText("Done");
            }
            else
            {
                this->boardModel->printMessage(" ");
                this->boardModel->printMessage(QString("You may still increase the City AV of %1 cities.")
                                              .arg(this->availableAdvancements));
                this->boardModel->printMessage("Choose another City to advance or press Done to Continue...");
                this->boardModel->printMessage(" ");
                this->boardModel->unsetActiveRegion();
                this->boardModel->setDoneText("Done");
            }
        }
        else
        {
            this->boardModel->printMessage("Advancing City AV cancelled.");
            this->boardModel->printMessage(" ");
            this->boardModel->unselectAllSelectedTribes();
            this->boardModel->unsetActiveRegion();
            this->boardModel->setDoneText("Done");
        }

        return this;
    }

    Instruction *next = new ReduceCityAVInstruction(this->boardModel);
    next->initInstruction();
    return next;
}

void AdvanceCityAVInstruction::ressourceChosen(QAbstractButton *button)
{
    QString ressourceDecimated;

    this->boardModel->setDoneButton(true);
    this->boardModel->setDoneText("Done");

    QMessageBox::ButtonRole buttonRole =  this->chooseRessource->buttonRole(button);

    switch(buttonRole)
    {
        case QMessageBox::YesRole:
            this->boardModel->refRegionModel(this->region)->setForest(false);
            ressourceDecimated = "Forest";
            break;
        case QMessageBox::NoRole:
            this->boardModel->refRegionModel(this->region)->setMountain(false);
            ressourceDecimated = "Mountain";
            break;
        case QMessageBox::ApplyRole:
            this->boardModel->refRegionModel(this->region)->setFarm(false);
            ressourceDecimated = "Farm";
            break;
        default: break;
    }

    this->availableAdvancements--;
    this->capitolBeyondFour = false;
    this->capitolAdvanced = true;
    this->boardModel->refRegionModel(this->region)->decimateTribes(this->toBePaid);
    this->boardModel->refActiveRegion()->increaseCityAV(1);

    this->boardModel->printMessage(QString("You decimated %1 Tribes in Region %2 and the %3 in Region %4 to increase the Capitol's City AV to %5.")
                                  .arg(this->toBePaid)
                                  .arg(this->boardModel->refActiveRegion()->getRegion())
                                  .arg(ressourceDecimated)
                                  .arg(this->region)
                                  .arg(this->boardModel->refActiveRegion()->getCityAV()));

    this->chooseRessource->deleteLater();

    return;
}
