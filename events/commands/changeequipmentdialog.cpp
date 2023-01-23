#include "changeequipmentdialog.h"
#include "ui_changeequipmentdialog.h"

#include "RXIO2/rpgdb.h"

ChangeEquipmentDialog::ChangeEquipmentDialog(RPGDB *db, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeEquipmentDialog)
{
    ui->setupUi(this);

    this->db = db;

    db->fill_combo(this->ui->combo_actor, RPGDB::ACTORS, true, 3);
    this->ui->combo_actor->setCurrentIndex(parameters.at(0).toInt()-1);
    this->update_combos();



    if (parameters.at(1).toInt() == 0)
    {
        this->ui->radio_weapon->setChecked(true);
        for (int i = 0; i < this->ui->combo_weapon->count(); i++)
        {
            if (this->ui->combo_weapon->itemData(i).toInt() == parameters.at(2).toInt())
                this->ui->combo_weapon->setCurrentIndex(i);
        }
    }
    else if (parameters.at(1).toInt() == 1)
    {
        this->ui->radio_shield->setChecked(true);
        for (int i = 0; i < this->ui->combo_shield->count(); i++)
        {
            if (this->ui->combo_shield->itemData(i).toInt() == parameters.at(2).toInt())
                this->ui->combo_shield->setCurrentIndex(i);
        }
    }
    else if (parameters.at(1).toInt() == 2)
    {
        this->ui->radio_helmet->setChecked(true);
        for (int i = 0; i < this->ui->combo_helmet->count(); i++)
        {
            if (this->ui->combo_helmet->itemData(i).toInt() == parameters.at(2).toInt())
                this->ui->combo_helmet->setCurrentIndex(i);
        }
    }
    else if (parameters.at(1).toInt() == 3)
    {
        this->ui->radio_body_armor->setChecked(true);
        for (int i = 0; i < this->ui->combo_body_armor->count(); i++)
        {
            if (this->ui->combo_body_armor->itemData(i).toInt() == parameters.at(2).toInt())
                this->ui->combo_body_armor->setCurrentIndex(i);
        }
    }
    else if (parameters.at(1).toInt() == 4)
    {
        this->ui->radio_accessory->setChecked(true);
        for (int i = 0; i < this->ui->combo_accessory->count(); i++)
        {
            if (this->ui->combo_accessory->itemData(i).toInt() == parameters.at(2).toInt())
                this->ui->combo_accessory->setCurrentIndex(i);
        }
    }

}

ChangeEquipmentDialog::~ChangeEquipmentDialog()
{
    delete ui;
}

void ChangeEquipmentDialog::update_combos()
{
    QJsonArray values = db->get_equipment_vars(this->ui->combo_actor->currentIndex()+1);

    int cid = 0;
    this->ui->combo_weapon->clear();
    this->ui->combo_weapon->addItem("(None)", 0);
    for (int i = 0; i < values.at(cid).toArray().count(); i++)
    {
        this->ui->combo_weapon->addItem(QString("%1: %2").arg(values.at(cid).toArray().at(i).toInt(), 3, 10, QChar('0')).arg(db->get_object_name(RPGDB::WEAPONS,values.at(cid).toArray().at(i).toInt())),values.at(cid).toArray().at(i).toInt());
    }


    cid = 1;
    this->ui->combo_shield->clear();
    this->ui->combo_shield->addItem("(None)", 0);
    for (int i = 0; i < values.at(cid).toArray().count(); i++)
    {
        this->ui->combo_shield->addItem(QString("%1: %2").arg(values.at(cid).toArray().at(i).toInt(), 3, 10, QChar('0')).arg(db->get_object_name(RPGDB::ARMORS,values.at(cid).toArray().at(i).toInt())),values.at(cid).toArray().at(i).toInt());
    }

    cid  = 2;
    this->ui->combo_helmet->clear();
    this->ui->combo_helmet->addItem("(None)", 0);
    for (int i = 0; i < values.at(cid).toArray().count(); i++)
    {
        this->ui->combo_helmet->addItem(QString("%1: %2").arg(values.at(cid).toArray().at(i).toInt(), 3, 10, QChar('0')).arg(db->get_object_name(RPGDB::ARMORS,values.at(cid).toArray().at(i).toInt())),values.at(cid).toArray().at(i).toInt());
    }

    cid = 3;
    this->ui->combo_body_armor->clear();
    this->ui->combo_body_armor->addItem("(None)", 0);
    for (int i = 0; i < values.at(cid).toArray().count(); i++)
    {
        this->ui->combo_body_armor->addItem(QString("%1: %2").arg(values.at(cid).toArray().at(i).toInt(), 3, 10, QChar('0')).arg(db->get_object_name(RPGDB::ARMORS,values.at(cid).toArray().at(i).toInt())),values.at(cid).toArray().at(i).toInt());
    }

    cid = 4;
    this->ui->combo_accessory->clear();
    this->ui->combo_accessory->addItem("(None)", 0);
    for (int i = 0; i < values.at(cid).toArray().count(); i++)
    {
        this->ui->combo_accessory->addItem(QString("%1: %2").arg(values.at(cid).toArray().at(i).toInt(), 3, 10, QChar('0')).arg(db->get_object_name(RPGDB::ARMORS,values.at(cid).toArray().at(i).toInt())),values.at(cid).toArray().at(i).toInt());
    }
}

void ChangeEquipmentDialog::on_button_cancel_clicked()
{
    this->close();
}


void ChangeEquipmentDialog::on_button_ok_clicked()
{
    QJsonArray p;

    p.append(this->ui->combo_actor->currentIndex()+1);
    if (this->ui->radio_weapon->isChecked())
    {
        p.append(0);
        p.append(this->ui->combo_weapon->currentData().toInt());
    }
    else if (this->ui->radio_shield->isChecked())
    {
        p.append(1);
        p.append(this->ui->combo_shield->currentData().toInt());
    }
    else if (this->ui->radio_helmet->isChecked())
    {
        p.append(2);
        p.append(this->ui->combo_helmet->currentData().toInt());
    }
    else if (this->ui->radio_body_armor->isChecked())
    {
        p.append(3);
        p.append(this->ui->combo_body_armor->currentData().toInt());
    }
    else if (this->ui->radio_accessory->isChecked())
    {
        p.append(4);
        p.append(this->ui->combo_accessory->currentData().toInt());
    }

    emit ok_clicked(p);
    this->close();
}


void ChangeEquipmentDialog::on_combo_actor_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    this->update_combos();
}

