#include "CardDialog.hpp"

#include <QLabel>
#include <QDesktopWidget>

CardDialog::CardDialog(BoardModel *boardModel, QWidget *parent)
    : QDialog(parent),
      boardModel(boardModel)
{
    this->setWindowTitle("Event Cards Overview");
    this->cardLayout = new QGridLayout(this);
    this->setLayout(this->cardLayout);

    this->cardsLeft = new QListWidget(this);
    this->cardsDiscarded = new QListWidget(this);

    this->cardLayout->addWidget(new QLabel("Event Cards Left", this), 0,0);
    this->cardLayout->addWidget(this->cardsLeft, 1,0);
    this->cardLayout->addWidget(new QLabel("Event Cards Discarded", this), 0,1);
    this->cardLayout->addWidget(this->cardsDiscarded, 1,1);

    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    this->setFixedSize(mainScreenSize.width() - 358, mainScreenSize.height() - 60);
    this->move(mainScreenSize.center() - this->rect().center());
}

void CardDialog::show()
{
    QList<const EventCard *> eventCards = this->boardModel->getEventCards();

    this->cardsLeft->clear();
    QList<const EventCard *> eventCardsLeft = this->boardModel->getEventCardsLeft();

    this->cardsLeft->setViewMode(QListView::IconMode);
    this->cardsLeft->setResizeMode(QListView::Adjust);
    this->cardsLeft->setIconSize(QSize(150,300));
    this->cardsLeft->setDragEnabled(false);

    for(int i = 0; i < eventCardsLeft.count(); ++i)
    {
        int cardNo = eventCards.indexOf(eventCardsLeft[i])+1;
        QPixmap card(QString(":/card%1").arg(cardNo));
        card = card.scaledToWidth(150, Qt::SmoothTransformation);
        QListWidgetItem *item = new QListWidgetItem(QIcon(card), QString("Card %1").arg(cardNo));
        item->setToolTip(QString("<img src=':/card%1'>").arg(cardNo));

        this->cardsLeft->addItem(item);
    }

    this->cardsDiscarded->clear();

    QList<const EventCard *> discardedEventCards = this->boardModel->getDiscardedEventCards();

    this->cardsDiscarded->setViewMode(QListView::IconMode);
    this->cardsDiscarded->setResizeMode(QListView::Adjust);
    this->cardsDiscarded->setIconSize(QSize(150,300));
    this->cardsDiscarded->setDragEnabled(false);

    for(int i = 0; i < discardedEventCards.count(); ++i)
    {
        int cardNo = eventCards.indexOf(discardedEventCards[i])+1;
        QPixmap card(QString(":/card%1").arg(cardNo));
        card = card.scaledToWidth(150, Qt::SmoothTransformation);
        QListWidgetItem *item = new QListWidgetItem(QIcon(card), QString("Card %1").arg(cardNo));
        item->setToolTip(QString("<img src=':/card%1'>").arg(cardNo));

        this->cardsDiscarded->addItem(item);
    }

    this->QDialog::show();
}
