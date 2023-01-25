#include "controlvariablesdialog.h"
#include "ui_controlvariablesdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapcontroller.h"

ControlVariablesDialog::ControlVariablesDialog(RPGDB *db, RPGMapController *mc, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlVariablesDialog)
{
    ui->setupUi(this);
    this->ui->widget_variable->setVariableWidget(db);
    this->ui->widget_var->setVariableWidget(db);

    db->fill_combo(this->ui->combo_item, RPGDB::ITEMS, true, 3);
    db->fill_combo(this->ui->combo_actor, RPGDB::ACTORS, true, 3);

    this->ui->combo_character->addItem("Player", -1);
    this->ui->combo_character->addItem("This event", 0);

    QJsonObject events = mc->current_map()->object().value("@events").toObject();
    //THX to https://stackoverflow.com/questions/54427846/how-can-i-do-a-numeric-sort-of-a-qstringlist-in-qt-4-6-2-where-qcollator-is-not
    QStringList keys = events.keys();
    keys.remove(keys.indexOf("RXClass"));

    QMap<int, QString> m;
    for (auto s : keys) m[s.toInt()] = s;
    keys = QStringList(m.values());

    for (QString key : keys)
        this->ui->combo_character->addItem(QString("%1: ").arg(key.toInt(), 3,10,QChar('0')) + events.value(key).toObject().value("@name").toString(), key.toInt());

    if (parameters.at(0).toInt() == parameters.at(1).toInt())
    {
        this->ui->radio_single->setChecked(true);
        this->ui->widget_variable->setValue(parameters.at(0).toInt());
    }
    else
    {
        this->ui->radio_batch->setChecked(true);
        this->ui->spin_from->setValue(parameters.at(0).toInt());
        this->ui->spin_to->setValue(parameters.at(1).toInt());
    }

    if (parameters.at(2).toInt() == 0)
        this->ui->radio_set->setChecked(true);
    else if (parameters.at(2).toInt() == 1)
        this->ui->radio_add->setChecked(true);
    else if (parameters.at(2).toInt() == 2)
        this->ui->radio_sub->setChecked(true);
    else if (parameters.at(2).toInt() == 3)
        this->ui->radio_mul->setChecked(true);
    else if (parameters.at(2).toInt() == 4)
        this->ui->radio_div->setChecked(true);
    else if (parameters.at(2).toInt() == 5)
        this->ui->radio_mod->setChecked(true);

    if (parameters.at(3).toInt() == 0)
    {
        this->ui->radio_constant->setChecked(true);
        this->ui->spin_constant->setValue(parameters.at(4).toInt());
    }
    else if (parameters.at(3).toInt() == 1)
    {
        this->ui->radio_variable->setChecked(true);
        this->ui->widget_var->setValue(parameters.at(4).toInt());
    }
    else if (parameters.at(3).toInt() == 2)
    {
        this->ui->radio_random->setChecked(true);
        int a = parameters.at(4).toInt();
        int b = parameters.at(5).toInt();
        if (a < b)
        {
            this->ui->spin_random_from->setValue(a);
            this->ui->spin_random_to->setValue(b);
        }
        else
        {
            this->ui->spin_random_from->setValue(b);
            this->ui->spin_random_to->setValue(a);
        }
    }
    else if (parameters.at(3).toInt() == 3)
    {
        this->ui->radio_item->setChecked(true);
        this->ui->combo_item->setCurrentIndex(parameters.at(4).toInt()-1);
    }
    else if (parameters.at(3).toInt() == 4)
    {
        this->ui->radio_actor->setChecked(true);
        this->ui->combo_actor->setCurrentIndex(parameters.at(4).toInt()-1);
        this->ui->combo_actor_param->setCurrentIndex(parameters.at(5).toInt());
    }
    else if (parameters.at(3).toInt() == 5)
    {
        this->ui->radio_enemy->setChecked(true);
        this->ui->combo_enemy->setCurrentIndex(parameters.at(4).toInt());
        this->ui->combo_enemy_param->setCurrentIndex(parameters.at(5).toInt());
    }
    else if (parameters.at(3).toInt() == 6)
    {
        this->ui->radio_character->setChecked(true);
        for (int i = 0; i < this->ui->combo_character->count(); i++)
        {
            if (this->ui->combo_character->itemData(i).toInt() == parameters.at(4).toInt())
                this->ui->combo_character->setCurrentIndex(i);
        }
        this->ui->combo_character_param->setCurrentIndex(parameters.at(5).toInt());
    }
    else if (parameters.at(3).toInt() == 7)
    {
        this->ui->radio_other->setChecked(true);
        this->ui->combo_other->setCurrentIndex(parameters.at(4).toInt());
    }
}

ControlVariablesDialog::~ControlVariablesDialog()
{
    delete ui;
}

void ControlVariablesDialog::on_button_cancel_clicked()
{
    this->close();
}


void ControlVariablesDialog::on_button_ok_clicked()
{
    QJsonArray p;

    if (this->ui->radio_single->isChecked())
    {
        p.append(this->ui->widget_variable->getValue());
        p.append(this->ui->widget_variable->getValue());
    }
    else if (this->ui->radio_batch->isChecked())
    {
        int a = this->ui->spin_from->value();
        int b = this->ui->spin_to->value();
        if (a < b)
        {
            p.append(a);
            p.append(b);
        }
        else
        {
            p.append(b);
            p.append(a);
        }
    }


    if (this->ui->radio_set->isChecked())
        p.append(0);
    else if (this->ui->radio_add->isChecked())
        p.append(1);
    else if (this->ui->radio_sub->isChecked())
        p.append(2);
    else if (this->ui->radio_mul->isChecked())
        p.append(3);
    else if (this->ui->radio_div->isChecked())
        p.append(4);
    else if (this->ui->radio_mod->isChecked())
        p.append(5);


    if (this->ui->radio_constant->isChecked())
    {
        p.append(0);
        p.append(this->ui->spin_constant->value());
    }
    else if (this->ui->radio_variable->isChecked())
    {
        p.append(1);
        p.append(this->ui->widget_var->getValue());
    }
    else if (this->ui->radio_random->isChecked())
    {
        p.append(2);
        int a = this->ui->spin_random_from->value();
        int b = this->ui->spin_random_to->value();
        if (a < b)
        {
            p.append(a);
            p.append(b);
        }
        else
        {
            p.append(b);
            p.append(a);
        }
    }
    else if (this->ui->radio_item->isChecked())
    {
        p.append(3);
        p.append(this->ui->combo_item->currentIndex()+1);
    }
    else if (this->ui->radio_actor->isChecked())
    {
        p.append(4);
        p.append(this->ui->combo_actor->currentIndex()+1);
        p.append(this->ui->combo_actor_param->currentIndex());
    }
    else if (this->ui->radio_enemy->isChecked())
    {
        p.append(5);
        p.append(this->ui->combo_enemy->currentIndex());
        p.append(this->ui->combo_enemy_param->currentIndex());
    }
    else if (this->ui->radio_character->isChecked())
    {
        p.append(6);
        p.append(this->ui->combo_character->currentData().toInt());
        p.append(this->ui->combo_character_param->currentIndex());
    }
    else if (this->ui->radio_other->isChecked())
    {
        p.append(7);
        p.append(this->ui->combo_other->currentIndex());
    }

    emit ok_clicked(p);
    this->close();
}

