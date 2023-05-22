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
        emit content_changed(this->get_data());
    });
    connect(this->ui->spin_lv_from, &QSpinBox::valueChanged, [=](){
        emit content_changed(this->get_data());
    });
    connect(this->ui->spin_lv_to, &QSpinBox::valueChanged, [=](){
        emit content_changed(this->get_data());
    });
    connect(this->ui->line_name, &QLineEdit::textChanged, [=](){
        emit content_changed(this->get_data());
    });
}

EncounterSlot::~EncounterSlot()
{
    delete ui;
}

QJsonArray EncounterSlot::get_data()
{
    QJsonArray array;
    array.append(this->ui->spin_prob->value());
    array.append(PBSFactory().create_symbol(this->ui->line_name->text()));
    array.append(this->ui->spin_lv_from->value());
    array.append(this->ui->spin_lv_to->value());
    return array;
}

void EncounterSlot::on_button_delete_clicked()
{
    emit slot_deleted();
}


