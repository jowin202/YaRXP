#include "editcommonevents.h"
#include "ui_editcommonevents.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeventlistcontroller.h"
#include "RXIO2/rpgeditorcontroller.h"

EditCommonEvents::EditCommonEvents(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditCommonEvents)
{
    ui->setupUi(this);
}

EditCommonEvents::~EditCommonEvents()
{
    delete ui;
    if (ecl != 0)
        delete ecl;
}

void EditCommonEvents::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ec->connect_string_to_text_field(RPGDB::COMMONEVENTS, "@name", this->ui->line_name);
    this->ec->connect_int_to_combo_box(RPGDB::COMMONEVENTS, "@trigger", this->ui->combo_trigger);

    this->ui->switch_widget->setSwitchWidget(ec->get_db());

    connect(this->ec, &RPGEditorController::current_common_event_changed, this, [=]() {
        this->ui->switch_widget->setValue(this->ec->obj_get_jsonvalue(RPGDB::COMMONEVENTS,"@switch_id").toInt());
        if (this->ecl != 0)
            this->ecl->fill_list(ec->obj_get_jsonvalue(RPGDB::COMMONEVENTS, "@list").toArray());
    });
    connect(this->ui->switch_widget->get_combo(), &QComboBox::currentIndexChanged, this, [=](int val){ this->ec->obj_set_jsonvalue(RPGDB::COMMONEVENTS, "@switch_id", val+1);});

    if (this->ecl == 0)
        this->ecl = new RPGEventListController(ec->get_db()->mc, this->ui->list);

}



void EditCommonEvents::on_combo_trigger_currentIndexChanged(int index)
{
    this->ui->switch_widget->setEnabled(index > 0);
}
