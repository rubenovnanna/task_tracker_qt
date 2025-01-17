#include "newtab.h"
#include "ui_newtab.h"
#include "UI/w_decklist/decklist.h"
#include "UI/w_deck/deck.h"
#include "Database/database.h"

#include "QMessageBox"

NewTab::NewTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewTab)
{
    ui->setupUi(this);
    cellNumberTotal = 0;
}

NewTab::~NewTab()
{
    delete ui;
}

void NewTab::on_pushButton_clicked()
{
    addNewCell();
}

void NewTab::addNewCell(QString cellText, QString cellName)
{
    if (cellNumberTotal == 4) {
        QMessageBox::warning(this, "Cant create", "Too much cards, maximum 4");
    } else {
        NewCell *newCell = new NewCell(this);
        ui->cellLayout->addWidget(newCell, cellNumberTotal, 0);
        connect(newCell,SIGNAL(closeThisCell(int)),this,SLOT(closeCell(int)));
        newCell->setAttribute(Qt::WA_DeleteOnClose, true);
        newCell->CellNumber = cellNumberTotal;
        newCell->setTextData(cellText);
        allCellPtr.append(newCell);
        cellNumberTotal++;
        DeckList::OpenedBoard->setMaxCardLabel(cellNumberTotal);
    }
}

void NewTab::closeCell(int index)
{
    allCellPtr[index]->close();
    for (int i=index+1;i<allCellPtr.length() ;i++ ) {
        allCellPtr[i]->CellNumber--;
    }
    cellNumberTotal--;
    allCellPtr.remove(index);

    DeckList::OpenedBoard->setMaxCardLabel(cellNumberTotal);
}
