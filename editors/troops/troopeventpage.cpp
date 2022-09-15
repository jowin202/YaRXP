#include "troopeventpage.h"
#include "ui_troopeventpage.h"
#include "trooppageconditiondialog.h"

#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgtrooppage.h"


TroopEventPage::TroopEventPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TroopEventPage)
{
    ui->setupUi(this);
}

TroopEventPage::~TroopEventPage()
{
    delete ui;
}

void TroopEventPage::setTroopPage(RPGSystem *system, RPGTroopPage *page)
{
    this->system = system;
    this->ui->list->set_data(system, &page->list);
    this->ui->list->fill_list();

    this->ui->combo_span->setCurrentIndex(page->span);

}


void TroopEventPage::on_button_condition_clicked()
{
    TroopPageConditionDialog *dialog = new TroopPageConditionDialog(system);
    dialog->show();
}
