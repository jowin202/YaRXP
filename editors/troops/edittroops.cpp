#include "edittroops.h"
#include "ui_edittroops.h"

#include "troopeventpage.h"

#include "RXIO/RXObjects/rpgsystem.h"
#include "RXIO/RXObjects/rpgtroop.h"

EditTroops::EditTroops(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditTroops)
{
    ui->setupUi(this);
}

EditTroops::~EditTroops()
{
    delete ui;
}

void EditTroops::set_troop(int n)
{
    if (this->system->troops_list.length() <= n) return;
    RPGTroop *current_troop = this->system->troops_list.at(n);

    this->ui->line_name->setText(current_troop->name);
    system->datasource.fill_list(this->ui->list_enemies, RPGSystem::ENEMIES, true, 3);

    this->ui->tab_pages->clear();
    for (int i = 0; i < current_troop->pages.length(); i++)
    {
        TroopEventPage *page = new TroopEventPage;
        page->setTroopPage(system,current_troop->pages.at(i), this->ui->label_troop_pic);
        this->ui->tab_pages->addTab(page, QString::number(i+1));
    }

    this->ui->label_troop_pic->setData(system, current_troop);
}

void EditTroops::on_button_arrange_clicked()
{
    this->ui->label_troop_pic->arrange();
}

void EditTroops::on_button_clear_clicked()
{
    this->ui->label_troop_pic->clear_troop();
    this->ui->label_troop_pic->redraw();
    this->ui->button_add->setEnabled(true); //always true after clear
}

void EditTroops::on_button_remove_clicked()
{
    this->ui->label_troop_pic->remove_current();
    this->ui->button_add->setEnabled(this->ui->label_troop_pic->get_member_count() < 8);
}

void EditTroops::on_button_add_clicked()
{
    this->ui->label_troop_pic->add_member(this->ui->list_enemies->currentRow()+1);
    this->ui->button_add->setEnabled(this->ui->label_troop_pic->get_member_count() < 8);
}
