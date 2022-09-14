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
        page->setTroopPage(system,current_troop->pages.at(i));
        this->ui->tab_pages->addTab(page, QString::number(i+1));
    }
}
