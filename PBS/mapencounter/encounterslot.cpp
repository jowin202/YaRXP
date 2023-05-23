#include "encounterslot.h"
#include "ui_encounterslot.h"

#include "../pbsfactory.h"

EncounterSlot::EncounterSlot(QJsonArray data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EncounterSlot)
{
    ui->setupUi(this);
    this->ui->spin_prob->setValue(data.at(0).toInt());
    this->ui->line_name->setText(data.at(1).toObject().value("symbol_value").toString());
    this->ui->spin_lv_from->setValue(data.at(2).toInt());
    this->ui->spin_lv_to->setValue(data.at(3).toInt());

    connect(this->ui->spin_prob, &QSpinBox::valueChanged, [=](){
        emit content_changed();
    });
    connect(this->ui->spin_lv_from, &QSpinBox::valueChanged, [=](){
        emit content_changed();
    });
    connect(this->ui->spin_lv_to, &QSpinBox::valueChanged, [=](){
        emit content_changed();
    });
    connect(this->ui->line_name, &QLineEdit::textChanged, [=](){
        emit content_changed();
    });
}

EncounterSlot::~EncounterSlot()
{
    delete ui;
}

QString EncounterSlot::get_name()
{
    return this->ui->line_name->text();
}

int EncounterSlot::get_prob()
{
    return this->ui->spin_prob->value();
}

int EncounterSlot::get_lv_from()
{
    return this->ui->spin_lv_from->value();
}

int EncounterSlot::get_lv_to()
{
    return this->ui->spin_lv_to->value();
}

void EncounterSlot::on_button_delete_clicked()
{
    emit slot_deleted();
}


