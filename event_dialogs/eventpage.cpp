#include "eventpage.h"
#include "ui_eventpage.h"

EventPage::EventPage(QWidget *parent, RPGEventPage *page, RPGSystem *system) :
    QWidget(parent),
    ui(new Ui::EventPage)
{
    ui->setupUi(this);
    this->page = page;

    this->ui->combo_switch1->setMaximumWidth(130);
    this->ui->combo_switch2->setMaximumWidth(130);
    this->ui->combo_variable->setMaximumWidth(130);
    this->ui->combo_self_switch->setMaximumWidth(130);
    this->ui->spin_variable->setMaximumWidth(130);

    if (system != 0)
    {
        for (int i = 0; i < system->switches_names.length(); i++)
        {
            this->ui->combo_switch1->addItem(QString::number(i+1).rightJustified(3,'0') + ": " + system->switches_names.at(i),i+1);
            this->ui->combo_switch2->addItem(QString::number(i+1).rightJustified(3,'0') + ": " + system->switches_names.at(i),i+1);
        }
        for (int i = 0; i < system->variable_names.length(); i++)
        {
            this->ui->combo_variable->addItem(QString::number(i+1).rightJustified(3,'0') + ": " + system->variable_names.at(i),i+1);
        }

        if (page->switch1_valid)
            this->ui->combo_switch1->setCurrentIndex(page->switch1_id-1);
        if (page->switch2_valid)
            this->ui->combo_switch2->setCurrentIndex(page->switch2_id-1);
        if (page->variable_valid)
        {
            this->ui->combo_variable->setCurrentIndex(page->variable_id-1);
            this->ui->spin_variable->setValue(page->variable_value);
        }

        this->ui->eventList->set_data(system, &page->list);
        this->ui->eventList->fill_list(); // happens only if settings are valid ... TODO: check
    }

    connect(this->ui->combo_move_type, SIGNAL(currentIndexChanged(int)), this, SLOT(move_type_changed(int)));
    this->ui->combo_move_type->setCurrentIndex(page->move_type);

    this->ui->combo_speed->setCurrentIndex(page->move_speed-1);
    this->ui->combo_freq->setCurrentIndex(page->move_frequency-1);


    this->ui->check_switch1->setChecked(page->switch1_valid);
    this->ui->combo_switch1->setEnabled(page->switch1_valid);

    this->ui->check_switch2->setChecked(page->switch2_valid);
    this->ui->combo_switch2->setEnabled(page->switch2_valid);

    this->ui->check_variable->setChecked(page->variable_valid);
    this->ui->combo_variable->setEnabled(page->variable_valid);
    this->ui->spin_variable->setEnabled(page->variable_valid);
    this->ui->spin_variable->setValue(page->variable_value);

    this->ui->check_self_switch->setChecked(page->self_switch_valid);
    this->ui->combo_self_switch->setEnabled(page->self_switch_valid);

    if (page->self_switch_ch == "A")
        this->ui->combo_self_switch->setCurrentIndex(0);
    else if (page->self_switch_ch == "B")
        this->ui->combo_self_switch->setCurrentIndex(1);
    else if (page->self_switch_ch == "C")
        this->ui->combo_self_switch->setCurrentIndex(2);
    else if (page->self_switch_ch == "D")
        this->ui->combo_self_switch->setCurrentIndex(3);

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

void EventPage::move_type_changed(int val)
{
    this->ui->button_move_route->setEnabled(val == 3); //only for move type == custom
}


void EventPage::on_button_add_command_clicked()
{
    EventCommandDialog *evcommdia = new EventCommandDialog;
    evcommdia->show();
}
