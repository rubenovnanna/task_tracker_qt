#include "deck.h"
#include "ui_deck.h"
#include <UI/w_login/loginwindow.h>
#include "UI/w_boardedit/boardedit.h"
#include "Database/database.h"
#include "QInputDialog"

Deck::Deck(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Deck)
{
    ui->setupUi(this);
}

Deck::~Deck()
{
    delete ui;
}

void Deck::setInfoLabels(QString Name, QString Type, QString Owners)
{
    ui->Name_label->setText("Name: " + Name);
    ui->Type_label->setText(Type);
    ui->Owners_label->setText("Owners: " + Owners);
}

void Deck::setMaxCardLabel(int Count)
{
    QString NewStr = "Cards: ";
    NewStr += QString::number(Count);
    NewStr += "/4";
    ui->maxCards_label->setText(NewStr);
}

void Deck::setBoardName(QString Name)
{
    ui->Name_label->text().clear();
    ui->Name_label->setText("Name: " + Name);
}

void Deck::setBoardPrivacyType(QString Type)
{
    ui->Type_label->text().clear();
    ui->Type_label->setText("Privacy type: " + Type);
}

void Deck::addBoardOwner(QString Owners)
{

    ui->Owners_label->text().clear();
    ui->Owners_label->setText("Owners: " + Owners);
}

void Deck::initTabs(QString ColumnName)
{
    NewTab *NewTabPtr = new NewTab(this);
    ui->tabWidget->addTab(NewTabPtr, ColumnName);
    NewTabPtr->setAttribute(Qt::WA_DeleteOnClose, true);

    allTabPtrs.append(NewTabPtr);
    for (auto Card : jsnBoard["Columns"]) {
        if (Card["ColumnName"] == ColumnName.toStdString()) {
            for (auto i : Card["Cards"]) {
                NewTabPtr->addNewCell(QString::fromStdString(i), "Cellname");
            }
        }
    }
}

void Deck::addTab(QString ColumnName)
{
    NewTab *NewTabPtr = new NewTab(this);
    ui->tabWidget->addTab(NewTabPtr, ColumnName);
    NewTabPtr->setAttribute(Qt::WA_DeleteOnClose, true);
    allTabPtrs.append(NewTabPtr);

    Database::AddNewColumn(QString::fromStdString(DeckList::OpenedBoard->jsnBoard["Name"]), ColumnName);
}

void Deck::on_btn_add_column_clicked()
{
    Database::AddNewColumn(QString::fromStdString(jsnBoard["Name"]), "NewColumn");
    addTab(QString("Tab %0").arg(ui->tabWidget->count()+1));
}

void Deck::on_tabWidget_tabCloseRequested(int index)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Confirmation", "Delete This Tab?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes){
        qDebug()<<"Removing tab of index:"<<index;
        allTabPtrs[index]->close();
        ui->tabWidget->removeTab(index);

        allTabPtrs.remove(index);
        Database::DeleteColumn(QString::fromStdString(jsnBoard["Name"]), index);
    }

    if(allTabPtrs.length()==0){
        addTab("Default");
        Database::AddNewColumn(QString::fromStdString(jsnBoard["Name"]), "Default");
        ui->statusBar->showMessage("Added Default Tab",3000);
    }
}

void Deck::on_btn_back_clicked()
{
    close();
    LoginWindow::WindowList->show();
}



void Deck::on_btn_edit_clicked()
{
    BoardEdit z;
    z.exec();
}


void Deck::on_btn_rename_tab_clicked()
{
    QString Text = QInputDialog::getText(this, "Enter new name", "Enter new name:", QLineEdit::Normal, "");
    std::cout << ui->tabWidget->tabText(ui->tabWidget->currentIndex()).toStdString() << std::endl;
    Database::RenameColumn(QString::fromStdString(jsnBoard["Name"]), ui->tabWidget->currentIndex(), Text);
    ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), Text);
}

