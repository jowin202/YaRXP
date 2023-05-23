#include "encounterwidget.h"
#include "ui_encounterwidget.h"


#include "../pbsfactory.h"
#include "encounterslot.h"

EncounterWidget::EncounterWidget(QJsonArray encounters_data, int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EncounterWidget)
{
    ui->setupUi(this);
    this->encounters_data = encounters_data;
    this->index = index;

    QJsonArray current = this->encounters_data.at(1).toObject().value("@step_chances").toObject().value("dict_list").toArray().at(index).toArray();
    this->ui->combo_type->setCurrentText(current.at(0).toObject().value("symbol_value").toString());
    this->ui->spin_step_chance->setValue(current.at(1).toInt());

    connect(this->ui->combo_type, &QComboBox::currentTextChanged, [=]() { emit data_changed(); });
    connect(this->ui->spin_step_chance, &QSpinBox::valueChanged, [=]() { emit data_changed(); });
    this->update_slots();
}

EncounterWidget::~EncounterWidget()
{
    delete ui;
}

QString EncounterWidget::type()
{
    return this->ui->combo_type->currentText();
}

int EncounterWidget::step_chance()
{
    return this->ui->spin_step_chance->value();
}

QJsonArray EncounterWidget::add_slot_data(QJsonArray encounter)
{
    PBSFactory factory;
    EncounterSlot *w;
    for (int i = 0; i < this->ui->encounter_layout->count(); i++)
    {
        if (this->ui->encounter_layout->itemAt(i) == 0) continue;
        if ((w=dynamic_cast<EncounterSlot*>(this->ui->encounter_layout->itemAt(i)->widget())) != nullptr)
        {
            encounter = factory.encounter_add_mon(encounter, this->type(), w->get_name(), w->get_prob(), w->get_lv_from(), w->get_lv_to());
        }
    }
    return encounter;
}

void EncounterWidget::update_slots()
{
    QLayoutItem *item;
     while ( (item = this->ui->encounter_layout->takeAt(0)) != 0)
         delete item->widget();

    QJsonArray types = encounters_data.at(1).toObject().value("@types").toObject().value("dict_list").toArray().at(index).toArray().at(1).toArray();
    for (int i = 0; i < types.count(); i++)
    {
        EncounterSlot *s = new EncounterSlot(types.at(i).toArray());
        this->ui->encounter_layout->addWidget(s);

        connect(s, &EncounterSlot::content_changed, [=](){ emit data_changed();});
        connect(s, &EncounterSlot::slot_deleted, [=](){ delete s; emit data_changed();});
    }
}


void EncounterWidget::on_button_add_slot_clicked()
{
    QJsonArray array;
    array.append(10); //prob
    array.append(PBSFactory().create_symbol("PKMN"));
    array.append(5); //from lv
    array.append(8); //to lvl
    EncounterSlot *s = new EncounterSlot(array);
    this->ui->encounter_layout->addWidget(s);
    connect(s, &EncounterSlot::content_changed, [=](){ emit data_changed();});
    connect(s, &EncounterSlot::slot_deleted, [=](){ delete s; emit data_changed();});
    emit data_changed();
}

