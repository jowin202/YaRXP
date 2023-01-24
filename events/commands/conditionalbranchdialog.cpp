#include "conditionalbranchdialog.h"
#include "ui_conditionalbranchdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapcontroller.h"


ConditionalBranchDialog::ConditionalBranchDialog(RPGDB *db, RPGMapController *mc, QJsonArray parameters, bool else_branch, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConditionalBranchDialog)
{
    ui->setupUi(this);

    this->ui->check_else_branch->setChecked(else_branch);

    maingroup.addButton(this->ui->radio_switch);
    maingroup.addButton(this->ui->radio_variable);
    maingroup.addButton(this->ui->radio_self_switch);
    maingroup.addButton(this->ui->radio_timer);
    maingroup.addButton(this->ui->radio_actor);
    maingroup.addButton(this->ui->radio_enemy);
    maingroup.addButton(this->ui->radio_character);
    maingroup.addButton(this->ui->radio_gold);
    maingroup.addButton(this->ui->radio_item);
    maingroup.addButton(this->ui->radio_weapon);
    maingroup.addButton(this->ui->radio_armor);
    maingroup.addButton(this->ui->radio_button);
    maingroup.addButton(this->ui->radio_script);

    variablegroup.addButton(this->ui->radio_var_constant);
    variablegroup.addButton(this->ui->radio_var_variable);

    actorgroup.addButton(this->ui->radio_actor_in_party);
    actorgroup.addButton(this->ui->radio_actor_name);
    actorgroup.addButton(this->ui->radio_actor_skill);
    actorgroup.addButton(this->ui->radio_actor_weapons);
    actorgroup.addButton(this->ui->radio_actor_armor);
    actorgroup.addButton(this->ui->radio_actor_state);

    enemygroup.addButton(this->ui->radio_enemy_appeared);
    enemygroup.addButton(this->ui->radio_enemy_state);



    this->db = db;
    this->mc = mc;

    for (int i = 1; i < db->get_switch_names().count(); i++)
        this->ui->combo_switch->addItem(QString("%1: %2").arg(i,4,10, QChar('0')).arg(db->get_switch_name(i)));

    for (int i = 1; i < db->get_variable_names().count(); i++)
    {
        this->ui->combo_variable->addItem(QString("%1: %2").arg(i,4,10, QChar('0')).arg(db->get_variable_name(i)));
        this->ui->combo_var_variable->addItem(QString("%1: %2").arg(i,4,10, QChar('0')).arg(db->get_variable_name(i)));
    }

    db->fill_combo(this->ui->combo_actor, RPGDB::ACTORS, true, 3);
    db->fill_combo(this->ui->combo_actor_skill, RPGDB::SKILLS, true, 3);
    db->fill_combo(this->ui->combo_actor_weapon, RPGDB::WEAPONS, true, 3);
    db->fill_combo(this->ui->combo_actor_armor, RPGDB::ARMORS, true, 3);
    db->fill_combo(this->ui->combo_actor_state, RPGDB::STATES, true, 3);
    db->fill_combo(this->ui->combo_enemy_state, RPGDB::STATES, true, 3);


    db->fill_combo(this->ui->combo_item, RPGDB::ITEMS, true, 3);
    db->fill_combo(this->ui->combo_weapon, RPGDB::WEAPONS, true, 3);
    db->fill_combo(this->ui->combo_armor, RPGDB::ARMORS, true, 3);

    for (int i = 0; i < 8; i++)
        this->ui->combo_enemy->addItem(QString("%1.").arg(i+1));


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
    {
        this->ui->combo_character->addItem(QString("%1: ").arg(key.toInt(), 3,10,QChar('0')) + events.value(key).toObject().value("@name").toString(), key.toInt());
        if (parameters.at(1).toInt() == key.toInt())
            this->ui->combo_character->setCurrentIndex(this->ui->combo_character->count()-1);
    }

    if (parameters.at(0).toInt() == 0)
    {
        //switch
        this->ui->radio_switch->setChecked(true);
        this->ui->combo_switch->setCurrentIndex(parameters.at(1).toInt()-1);
        this->ui->combo_switch_state->setCurrentIndex(parameters.at(2).toInt());
    }
    else if (parameters.at(0).toInt() == 1)
    {
        //Variable
        this->ui->radio_variable->setChecked(true);
        this->ui->combo_variable->setCurrentIndex(parameters.at(1).toInt()-1);

        if (parameters.at(2).toInt() == 0) //Constant
        {
            this->ui->radio_var_constant->setChecked(true);
            this->ui->spin_var_const->setValue(parameters.at(3).toInt());
            this->ui->combo_variable_op->setCurrentIndex(parameters.at(4).toInt());
        }
        else //variable
        {
            this->ui->radio_var_variable->setChecked(true);
            this->ui->combo_var_variable->setCurrentIndex(parameters.at(3).toInt()-1);
            this->ui->combo_variable_op->setCurrentIndex(parameters.at(4).toInt());
        }
    }
    else if (parameters.at(0).toInt() == 2)
    {
        //self switch
        this->ui->radio_self_switch->setChecked(true);
        if (parameters.at(1).toString() == "A")
            this->ui->combo_self_switch->setCurrentIndex(0);
        else if (parameters.at(1).toString() == "B")
            this->ui->combo_self_switch->setCurrentIndex(1);
        else if (parameters.at(1).toString() == "C")
            this->ui->combo_self_switch->setCurrentIndex(2);
        else // "D"
            this->ui->combo_self_switch->setCurrentIndex(3);

        this->ui->combo_self_switch_state->setCurrentIndex(parameters.at(2).toInt());
    }
    else if (parameters.at(0).toInt() == 3)
    {
        //Timer
        this->ui->radio_timer->setChecked(true);
        this->ui->spin_timer_min->setValue(parameters.at(1).toInt() / 60);
        this->ui->spin_timer_sec->setValue(parameters.at(1).toInt() % 60);
        this->ui->spin_timer_comp->setCurrentIndex(parameters.at(2).toInt());
    }
    else if (parameters.at(0).toInt() == 4)
    {
        //Actors
        this->ui->radio_actor->setChecked(true);
        this->ui->combo_actor->setCurrentIndex(parameters.at(1).toInt()-1);

        if (parameters.at(2).toInt() == 0)
            this->ui->radio_actor_in_party->setChecked(true);
        else if (parameters.at(2).toInt() == 1)
        {
            this->ui->radio_actor_name->setChecked(true);
            this->ui->line_actor_name->setText(parameters.at(3).toString());
        }
        else if (parameters.at(2).toInt() == 2)
        {
            this->ui->radio_actor_skill->setChecked(true);
            this->ui->combo_actor_skill->setCurrentIndex(parameters.at(3).toInt()-1);
        }
        else if (parameters.at(2).toInt() == 3)
        {
            this->ui->radio_actor_weapons->setChecked(true);
            this->ui->combo_actor_weapon->setCurrentIndex(parameters.at(3).toInt()-1);
        }
        else if (parameters.at(2).toInt() == 4)
        {
            this->ui->radio_actor_armor->setChecked(true);
            this->ui->combo_actor_armor->setCurrentIndex(parameters.at(3).toInt()-1);
        }
        else if (parameters.at(2).toInt() == 5)
        {
            this->ui->radio_actor_state->setChecked(true);
            this->ui->combo_actor_state->setCurrentIndex(parameters.at(3).toInt()-1);
        }
    }
    else if (parameters.at(0).toInt() == 5)
    {
        //Enemies
        this->ui->radio_enemy->setChecked(true);
        this->ui->combo_enemy->setCurrentIndex(parameters.at(1).toInt());
        if (parameters.at(2).toInt() == 0)
            this->ui->radio_enemy_appeared->setChecked(true);
        else //state
            this->ui->radio_enemy_state->setChecked(true);

        this->ui->combo_enemy_state->setCurrentIndex(parameters.at(3).toInt()-1);
    }
    else if (parameters.at(0).toInt() == 6)
    {
        //Character
        this->ui->radio_character->setChecked(true);
        this->ui->combo_character_direction->setCurrentIndex(parameters.at(2).toInt()/2-1);

        if (parameters.at(1).toInt() == 0)
            this->ui->combo_character->setCurrentIndex(1); //this event is coded as value 0
        //other events are coded before the loop
    }
    else if (parameters.at(0).toInt() == 7)
    {
        //gold
        this->ui->radio_gold->setChecked(true);
        this->ui->spin_gold->setValue(parameters.at(1).toInt());
        this->ui->combo_gold_comp->setCurrentIndex(parameters.at(2).toInt());
    }
    else if (parameters.at(0).toInt() == 8)
    {
        //item
        this->ui->radio_item->setChecked(true);
        this->ui->combo_item->setCurrentIndex(parameters.at(1).toInt()-1);
    }
    else if (parameters.at(0).toInt() == 9)
    {
        //weapon
        this->ui->radio_weapon->setChecked(true);
        this->ui->combo_weapon->setCurrentIndex(parameters.at(1).toInt()-1);
    }
    else if (parameters.at(0).toInt() == 10)
    {
        //armor
        this->ui->radio_armor->setChecked(true);
        this->ui->combo_armor->setCurrentIndex(parameters.at(1).toInt()-1);
    }
    else if (parameters.at(0).toInt() == 11)
    {
        //button
        this->ui->radio_button->setChecked(true);
        int index = (parameters.at(1).toInt() < 10 ? parameters.at(1).toInt()/2-1 : parameters.at(1).toInt()-7);
        this->ui->combo_button->setCurrentIndex(index);
    }
    else if (parameters.at(0).toInt() == 12)
    {
        //script
        this->ui->radio_script->setChecked(true);
        this->ui->line_script->setText(parameters.at(1).toString());
    }

}

ConditionalBranchDialog::~ConditionalBranchDialog()
{
    delete ui;
}

void ConditionalBranchDialog::on_button_ok_clicked()
{
    QJsonArray p;

    if (this->ui->radio_switch->isChecked())
    {
        p.append(0);
        p.append(this->ui->combo_switch->currentIndex()+1);
        p.append(this->ui->combo_switch_state->currentIndex());
    }
    else if (this->ui->radio_variable->isChecked())
    {
        p.append(1);
        p.append(this->ui->combo_variable->currentIndex()+1);
        if (this->ui->radio_var_constant->isChecked())
        {
            p.append(0);
            p.append(this->ui->spin_var_const->value());
        }
        else //variable
        {
            p.append(1);
            p.append(this->ui->combo_var_variable->currentIndex()+1);
        }
        p.append(this->ui->combo_variable_op->currentIndex());
    }
    else if (this->ui->radio_self_switch->isChecked())
    {
        p.append(2);
        if (this->ui->combo_self_switch->currentIndex() == 0)
            p.append("A");
        else if (this->ui->combo_self_switch->currentIndex() == 1)
            p.append("B");
        else if (this->ui->combo_self_switch->currentIndex() == 2)
            p.append("C");
        else if (this->ui->combo_self_switch->currentIndex() == 3)
            p.append("D");
        p.append(this->ui->combo_self_switch_state->currentIndex());
    }
    else if (this->ui->radio_timer->isChecked())
    {
        p.append(3);
        p.append(this->ui->spin_timer_min->value()*60 + this->ui->spin_timer_sec->value());
        p.append(this->ui->spin_timer_comp->currentIndex());
    }
    else if (this->ui->radio_actor->isChecked())
    {
        p.append(4);
        p.append(this->ui->combo_actor->currentIndex()+1);
        if (this->ui->radio_actor_in_party->isChecked())
            p.append(0);
        else if (this->ui->radio_actor_name->isChecked())
        {
            p.append(1);
            p.append(this->ui->line_actor_name->text());
        }
        else if (this->ui->radio_actor_skill->isChecked())
        {
            p.append(2);
            p.append(this->ui->combo_actor_skill->currentIndex()+1);
        }
        else if (this->ui->radio_actor_weapons->isChecked())
        {
            p.append(3);
            p.append(this->ui->combo_actor_weapon->currentIndex()+1);
        }
        else if (this->ui->radio_actor_armor->isChecked())
        {
            p.append(4);
            p.append(this->ui->combo_actor_armor->currentIndex()+1);
        }
        else if (this->ui->radio_actor_state->isChecked())
        {
            p.append(5);
            p.append(this->ui->combo_actor_state->currentIndex()+1);
        }
    }
    else if (this->ui->radio_enemy->isChecked())
    {
        p.append(5);
        p.append(this->ui->combo_enemy->currentIndex());

        if (this->ui->radio_enemy_appeared->isChecked())
            p.append(0); //enemy appeared
        else
            p.append(1);//enemy state inflicted

        p.append(this->ui->combo_enemy_state->currentIndex()+1); //always append this, even if not needed
    }
    else if (this->ui->radio_character->isChecked())
    {
        p.append(6);
        p.append(this->ui->combo_character->currentData().toInt());
        p.append((this->ui->combo_character_direction->currentIndex()+1)*2);
    }
    else if (this->ui->radio_gold->isChecked())
    {
        p.append(7);
        p.append(this->ui->spin_gold->value());
        p.append(this->ui->combo_gold_comp->currentIndex());
    }
    else if (this->ui->radio_item->isChecked())
    {
        p.append(8);
        p.append(this->ui->combo_item->currentIndex()+1);
    }
    else if (this->ui->radio_weapon->isChecked())
    {
        p.append(9);
        p.append(this->ui->combo_weapon->currentIndex()+1);
    }
    else if (this->ui->radio_armor->isChecked())
    {
        p.append(10);
        p.append(this->ui->combo_armor->currentIndex()+1);
    }
    else if (this->ui->radio_button->isChecked())
    {
        p.append(11);
        int index = this->ui->combo_button->currentIndex();
        if (index <= 3)
            p.append((index+1)*2);
        else
            p.append(index+7);
    }
    else if (this->ui->radio_script->isChecked())
    {
        p.append(12);
        p.append(this->ui->line_script->text());
    }

    emit ok_clicked(p, this->ui->check_else_branch->isChecked());

    this->close();
}


void ConditionalBranchDialog::on_button_cancel_clicked()
{
    this->close();
}

