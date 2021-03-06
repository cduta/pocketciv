#include "AssignCapitolInstruction.hpp"

#include "Instruction/AdvanceCityAVInstruction.hpp"
#include "Instruction/ReduceCityAVInstruction.hpp"

AssignCapitolInstruction::AssignCapitolInstruction(BoardModel *boardModel)
    : boardModel(boardModel)
{}

void AssignCapitolInstruction::initInstruction()
{
    this->boardModel->printMessage("Advance (CENTRALIZED GOVERNMENT):");

    if(this->boardModel->hasCity())
    {
        this->boardModel->printMessage("You MAY assign a City in the Empire to become the Capitol.");
    }
    else
    {
        this->boardModel->printMessage("The Empire has no cities.");
        this->boardModel->printMessage("Therefore, no Capital can be assigned.");
    }

    this->boardModel->printMessage(" ");
    this->boardModel->printMessage("Press Done to Continue...");
    this->boardModel->printMessage(" ");
    return;
}

Instruction *AssignCapitolInstruction::triggerHex(Qt::MouseButton button, int x, int y)
{
    RegionModel *regionModel = this->boardModel->refRegionModel(x,y);

    if(regionModel == NULL)
    {
        return this;
    }

    if(button == Qt::LeftButton)
    {
        if(this->boardModel->hasCapitolAssigned() && this->boardModel->refCapitolRegion() != regionModel)
        {
            this->boardModel->refCapitolRegion()->setCapitolRegion(false);
        }

        regionModel->toggleCapitol();
    }
    else if(button == Qt::RightButton)
    {
        if(this->boardModel->hasCapitolAssigned())
        {
            this->boardModel->refCapitolRegion()->setCapitolRegion(false);
        }
    }

    return this;
}

Instruction *AssignCapitolInstruction::triggerDone()
{
    Instruction *next;

    if(this->boardModel->hasAdvanceAquired(AdvanceModel::CIVIL_SERVICE) ||
       this->boardModel->hasAdvanceAquired(AdvanceModel::MASONRY) ||
       this->boardModel->hasAdvanceAquired(AdvanceModel::SLAVE_LABOR))
    {
        next = new AdvanceCityAVInstruction(this->boardModel);
    }
    else
    {
        this->boardModel->printMessage("To Advance City AV, you need at least one of the following advances:");
        this->boardModel->printMessage("CIVIL SERVICE, MASONRY or SLAVE LABOR.");
        this->boardModel->printMessage(" ");
        next = new ReduceCityAVInstruction(this->boardModel);
    }

    if(this->boardModel->hasCity() && !this->boardModel->hasCapitolAssigned())
    {
        this->boardModel->printMessage("No City chosen...");
    }
    else
    {
        this->boardModel->printMessage(QString("The City in Region %1 has become the Capitol.")
                                      .arg(this->boardModel->refCapitolRegion()->getRegion()));
        this->boardModel->printMessage("Any City AV beyond 4 AV can only be reache with Tribes from that Region and by decimating a Forest, Mountain or Farm anywhere in the Empire.");
    }

    this->boardModel->printMessage(" ");
    next->initInstruction();
    return next;
}
