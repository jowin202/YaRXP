#include "eventpage.h"
#include "ui_eventpage.h"

#include "dialogs/imagedialog.h"
#include "RXIO/RXObjects/rpgsystem.h"

EventPage::EventPage(QWidget *parent, RPGEventPage *page, RPGSystem *system) :
    QWidget(parent),
    ui(new Ui::EventPage)
{
    ui->setupUi(this);
    this->page = page;


    this->ui->label_graphic->set_data_from_page(system, page);

    if (system != 0)
    {
        this->ui->widget_switch1->setSwitchWidget(system);
        this->ui->widget_switch2->setSwitchWidget(system);
        this->ui->widget_variable->setVariableWidget(system);

        this->ui->eventList->set_data(system, &page->list);
        this->ui->eventList->fill_list(); // happens only if settings are valid ... TODO: check
    }

    connect(this->ui->combo_move_type, SIGNAL(currentIndexChanged(int)), this, SLOT(move_type_changed(int)));
    this->ui->combo_move_type->setCurrentIndex(page->move_type);

    this->ui->combo_speed->setCurrentIndex(page->move_speed-1);
    this->ui->combo_freq->setCurrentIndex(page->move_frequency-1);


    this->ui->check_switch1->setChecked(page->switch1_valid);
    this->ui->widget_switch1->setValue(page->switch1_id);

    this->ui->check_switch2->setChecked(page->switch2_valid);
    this->ui->widget_switch2->setValue(page->switch2_id);

    this->ui->check_variable->setChecked(page->variable_valid);
    this->ui->widget_variable->setValue(page->variable_id);
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

void EventPage::open_page_graphic_dialog()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::EVENTPAGE, page->character_name, 0);

    dialog->show();
}
