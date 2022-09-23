#include "troopeventpage.h"
#include "ui_troopeventpage.h"
#include "trooppageconditiondialog.h"

#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgtrooppage.h"

#include "trooppiclabel.h"

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

void TroopEventPage::setTroopPage(RPGSystem *system, RPGTroopPage *page, TroopPicLabel *label)
{
    this->system = system;
    this->page = page;
    this->troop_pic_label = label;
    this->ui->list->set_data(system, &page->list);
    this->ui->list->fill_list();
    this->ui->combo_span->setCurrentIndex(page->span);

    this->ui->line_condition->setText(page->get_condition_as_string(system,label->get_enemies_list()));

}


void TroopEventPage::on_button_condition_clicked()
{
    TroopPageConditionDialog *dialog = new TroopPageConditionDialog(system);
    dialog->setPage(page, this->troop_pic_label->get_enemies_list());
    dialog->show();
}
