#include "eventpage.h"
#include "ui_eventpage.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/factory.h"
#include "RXIO2/rpgmapcontroller.h"
#include "RXIO2/rpgeventlistcontroller.h"
#include "dialogs/imagedialog.h"
#include "events/commands/moveroutedialog.h"

EventPage::EventPage(QJsonObject page, RPGMapController *mc, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventPage)
{
    ui->setupUi(this);

    this->mc = mc;
    this->db = mc->getDB();


    this->ui->check_switch1->setChecked(page.value("@condition").toObject().value("@switch1_valid").toBool());
    this->ui->check_switch2->setChecked(page.value("@condition").toObject().value("@switch2_valid").toBool());
    this->ui->check_variable->setChecked(page.value("@condition").toObject().value("@variable_valid").toBool());
    this->ui->check_self_switch->setChecked(page.value("@condition").toObject().value("@self_switch_valid").toBool());

    this->ui->widget_switch1->setSwitchWidget(db);
    this->ui->widget_switch1->setValue(page.value("@condition").toObject().value("@switch1_id").toInt());
    this->ui->widget_switch2->setSwitchWidget(db);
    this->ui->widget_switch2->setValue(page.value("@condition").toObject().value("@switch2_id").toInt());

    this->ui->widget_variable->setVariableWidget(db);
    this->ui->widget_variable->setValue(page.value("@condition").toObject().value("@variable_id").toInt());
    this->ui->spin_variable->setValue(page.value("@condition").toObject().value("@variable_value").toInt());


    if (page.value("@condition").toObject().value("@self_switch_ch").toString() == "A")
        this->ui->combo_self_switch->setCurrentIndex(0);
    else if (page.value("@condition").toObject().value("@self_switch_ch").toString() == "B")
        this->ui->combo_self_switch->setCurrentIndex(1);
    else if (page.value("@condition").toObject().value("@self_switch_ch").toString() == "C")
        this->ui->combo_self_switch->setCurrentIndex(2);
    else if (page.value("@condition").toObject().value("@self_switch_ch").toString() == "D")
        this->ui->combo_self_switch->setCurrentIndex(3);


    this->ui->label_graphic->set_data_from_page(this->db,
                                                page.value("@graphic").toObject().value("@character_name").toString(),
                                                page.value("@graphic").toObject().value("@character_hue").toInt(),
                                                page.value("@graphic").toObject().value("@pattern").toInt(),
                                                page.value("@graphic").toObject().value("@direction").toInt(),
                                                page.value("@graphic").toObject().value("@opacity").toInt(),
                                                page.value("@graphic").toObject().value("@blend_type").toInt(),
                                                page.value("@graphic").toObject().value("@tile_id").toInt(),
                                                mc->current_map()->object().value("@tileset_id").toInt());


    this->ui->check_always_on_top->setChecked(page.value("@always_on_top").toBool());
    this->ui->check_direction_fix->setChecked(page.value("@direction_fix").toBool());
    this->ui->check_stop_animation->setChecked(page.value("@step_anime").toBool());
    this->ui->check_move_animation->setChecked(page.value("@walk_anime").toBool());
    this->ui->check_through->setChecked(page.value("@through").toBool());


    this->ui->combo_move_type->setCurrentIndex(page.value("@move_type").toInt());
    this->ui->combo_speed->setCurrentIndex(page.value("@move_speed").toInt()-1);
    this->ui->combo_freq->setCurrentIndex(page.value("@move_frequency").toInt()-1);

    switch(page.value("@trigger").toInt())
    {
    case 0:
        this->ui->radio_action_button->setChecked(true);
        break;
    case 1:
        this->ui->radio_player_touch->setChecked(true);
        break;
    case 2:
        this->ui->radio_event_touch->setChecked(true);
        break;
    case 3:
        this->ui->radio_autorun->setChecked(true);
        break;
    case 4:
        this->ui->radio_parallel_process->setChecked(true);
    }

    this->move_route = page.value("@move_route").toObject();

    this->evc = new RPGEventListController(this->mc, this->ui->eventList);
    this->evc->fill_list(page.value("@list").toArray());
}

EventPage::EventPage(RPGMapController *mc, QWidget *parent):
    QWidget(parent),
    ui(new Ui::EventPage)
{
    ui->setupUi(this);
    this->mc = mc;
    this->db = mc->getDB();
    //blank page
    this->ui->widget_switch1->setSwitchWidget(db);
    this->ui->widget_switch2->setSwitchWidget(db);
    this->ui->widget_variable->setVariableWidget(db);
}

EventPage::~EventPage()
{
    delete ui;
}

QJsonObject EventPage::getPage()
{
    int trigger = 0; //action button
    if (this->ui->radio_player_touch->isChecked()) trigger = 1;
    else if (this->ui->radio_event_touch->isChecked()) trigger = 2;
    else if (this->ui->radio_autorun->isChecked()) trigger= 3;
    else if (this->ui->radio_parallel_process->isChecked()) trigger = 4;

    return Factory().create_event_page(
                this->ui->check_always_on_top->isChecked(),
                this->ui->check_direction_fix->isChecked(),
                this->ui->check_stop_animation->isChecked(),
                this->ui->check_through->isChecked(),
                this->ui->check_move_animation->isChecked(),
                this->ui->combo_move_type->currentIndex(),
                this->ui->combo_speed->currentIndex()+1,
                this->ui->combo_freq->currentIndex()+1,
                trigger,
                Factory().create_page_condition(this->ui->check_self_switch->isChecked(),
                                                this->ui->check_switch1->isChecked(),
                                                this->ui->check_switch2->isChecked(),
                                                this->ui->check_variable->isChecked(),
                                                this->ui->combo_self_switch->currentIndex(),
                                                this->ui->widget_switch1->getValue(),
                                                this->ui->widget_switch2->getValue(),
                                                this->ui->widget_variable->getValue(),
                                                this->ui->spin_variable->value()),
                this->evc->get_list(),
                this->ui->label_graphic->get_data_from_page(),
                this->move_route);
}


void EventPage::on_button_add_command_clicked()
{
    EventCommandDialog *evcommdia = new EventCommandDialog(this->ui->eventList,mc,0,this->ui->eventList->currentRow());
    evcommdia->show();
}


void EventPage::on_combo_move_type_currentIndexChanged(int index)
{
    this->ui->button_move_route->setEnabled(index == 3);
}


void EventPage::on_button_move_route_clicked()
{
    QJsonArray p;
    p.append(-404); //see move route dialog
    p.append(this->move_route);
    MoveRouteDialog *dialog = new MoveRouteDialog(db,mc,p);
    dialog->show();
    connect(dialog, &MoveRouteDialog::ok_clicked, [=](QJsonArray p)
    {
       this->move_route = p.at(1).toObject();
    });
}

