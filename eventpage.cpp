#include "eventpage.h"
#include "ui_eventpage.h"

EventPage::EventPage(QWidget *parent, RPGEventPage *page) :
    QWidget(parent),
    ui(new Ui::EventPage)
{
    ui->setupUi(this);
    this->page = page;

    this->ui->check_switch1->setEnabled(page->condition->switch1_valid);
    this->ui->combo_switch1->setEnabled(page->condition->switch1_valid);

    this->ui->check_switch2->setEnabled(page->condition->switch2_valid);
    this->ui->combo_switch2->setEnabled(page->condition->switch2_valid);

    this->ui->check_variable->setEnabled(page->condition->variable_valid);
    this->ui->combo_variable->setEnabled(page->condition->variable_valid);
    this->ui->spin_variable->setEnabled(page->condition->variable_valid);
    this->ui->spin_variable->setValue(page->condition->variable_value);

    this->ui->check_self_switch->setEnabled(page->condition->self_switch_valid);
    this->ui->combo_self_switch->setEnabled(page->condition->self_switch_valid);

    this->ui->combo_self_switch->setCurrentIndex(page->condition->self_switch_ch-1);

    this->ui->check_move_animation->setChecked(page->walk_anime);
    this->ui->check_stop_animation->setChecked(page->step_anime);
    this->ui->check_direction_fix->setChecked(page->direction_fix);
    this->ui->check_through->setChecked(page->through);
    this->ui->check_always_on_top->setChecked(page->always_on_top);


    //verify this
    if (page->trigger == 0)
        this->ui->radio_action_button->setChecked(true);
    else if (page->trigger == 1)
        this->ui->radio_player_touch->setChecked(true);
    else if (page->trigger == 2)
        this->ui->radio_event_touch->setChecked(true);
    else if (page->trigger == 3)
        this->ui->radio_autorun->setChecked(true);
    else if (page->trigger == 4)
        this->ui->radio_parallel_process->setChecked(true);


}

EventPage::~EventPage()
{
    delete ui;
}
