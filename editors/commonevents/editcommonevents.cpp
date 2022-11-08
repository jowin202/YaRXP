#include "editcommonevents.h"
#include "ui_editcommonevents.h"

#include "RXIO/RXObjects/rpgsystem.h"

EditCommonEvents::EditCommonEvents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditCommonEvents)
{
    ui->setupUi(this);
}

EditCommonEvents::~EditCommonEvents()
{
    delete ui;
}

void EditCommonEvents::setSystem(RPGSystem *system)
{
    this->system = system;
    this->ui->switch_widget->setSwitchWidget(system);
}

void EditCommonEvents::set_commonevent(int n)
{
    this->ui->list->clear();
    if (this->system->common_events_list.length() <= n) return;
    RPGCommonEvent *current_common_event = this->system->common_events_list.at(n);

    this->ui->line_name->setText(current_common_event->name);
    this->ui->combo_trigger->setCurrentIndex(current_common_event->trigger);
    this->ui->switch_widget->setValue(current_common_event->switch_id);

    this->ui->list->set_data(system, &current_common_event->list);
    this->ui->list->import_list();
}

void EditCommonEvents::on_combo_trigger_currentIndexChanged(int index)
{
    this->ui->switch_widget->setEnabled(index > 0);
}
