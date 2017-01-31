#include "GloryDialog.hpp"

GloryDialog::GloryDialog(BoardModel *boardModel, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel)
{
    this->setWindowTitle("Glory Score Overview");
    this->gridLayout = new QGridLayout(this);
    this->setLayout(this->gridLayout);

    QFont font("monospace");
    this->gloryOverview = new QPlainTextEdit(this);
    this->gloryOverview->setFont(font);
    this->gloryOverview->setReadOnly(true);

    QList<int> gloryByEra = this->boardModel->getGloryScoreByEra();

    this->gloryOverview->insertPlainText(QString("### Glory Score ###\n"));

    for(int i = 0; i < gloryByEra.count(); ++i)
    {
        this->gloryOverview->insertPlainText(QString("Era %1: %2 VP\n").arg(i+1).arg(gloryByEra[i]));
    }

    this->gloryOverview->insertPlainText("---------\n");
    this->gloryOverview->insertPlainText(QString("Total Glory Score: %1 VP\n").arg(this->boardModel->getGloryScore()));

    this->gridLayout->addWidget(this->gloryOverview, 0,0);
}
