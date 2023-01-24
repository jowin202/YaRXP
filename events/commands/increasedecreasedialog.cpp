#include "increasedecreasedialog.h"
#include "ui_increasedecreasedialog.h"

#include "RXIO2/rpgdb.h"


IncreaseDecreaseDialog::IncreaseDecreaseDialog(RPGDB *db, int code, QJsonArray parameters, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IncreaseDecreaseDialog)
{
    ui->setupUi(this);

    this->code = code;
    this->ui->widget_variable->setVariableWidget(db);

    if (code == 125) //change gold
    {
        this->setWindowTitle("Change Gold");
        this->ui->label->setVisible(false);
        this->ui->combo->setVisible(false);
        this->ui->label_param->setVisible(false);
        this->ui->combo_param->setVisible(false);
        this->ui->check_battle_knockout->setVisible(false);

        if (parameters.at(0).toInt() == 0) this->ui->radio_increase->setChecked(true);
        else this->ui->radio_decrease->setChecked(true);

        if (parameters.at(1).toInt() == 0) //constant
        {
            this->ui->radio_constant->setChecked(true);
            this->ui->spin_constant->setValue(parameters.at(2).toInt());
        }
        else //variable
        {
            this->ui->radio_variable->setChecked(true);
            this->ui->widget_variable->setValue(parameters.at(2).toInt());
        }
    }
    else if (code == 126 || code == 127 || code == 128)
    {
        this->ui->label_param->setVisible(false);
        this->ui->combo_param->setVisible(false);
        this->ui->check_battle_knockout->setVisible(false);

        if (code == 126)
        {
            this->setWindowTitle("Change Items");
            this->ui->label->setText("Item:");
            db->fill_combo(this->ui->combo, RPGDB::ITEMS, true, 3);
        }
        else if (code == 127)
        {
            this->setWindowTitle("Change Weapons");
            this->ui->label->setText("Weapon:");
            db->fill_combo(this->ui->combo, RPGDB::WEAPONS, true, 3);
        }
        else if (code == 128)
        {
            this->setWindowTitle("Change Armor");
            this->ui->label->setText("Armor:");
            db->fill_combo(this->ui->combo, RPGDB::ARMORS, true, 3);
        }

        this->ui->combo->setCurrentIndex(parameters.at(0).toInt()-1);

        if (parameters.at(1).toInt() == 0) this->ui->radio_increase->setChecked(true);
        else this->ui->radio_decrease->setChecked(true);

        if (parameters.at(2).toInt() == 0) //constant
        {
            this->ui->radio_constant->setChecked(true);
            this->ui->spin_constant->setValue(parameters.at(3).toInt());
        }
        else //variable
        {
            this->ui->radio_variable->setChecked(true);
            this->ui->widget_variable->setValue(parameters.at(3).toInt());
        }
    }
    else if (code == 311 || code == 312 || code == 315 || code == 316) //Change HP, SP, EXP, LEVEL
    {
        this->ui->label_param->setVisible(false);
        this->ui->combo_param->setVisible(false);
        if (code == 311)
            this->setWindowTitle("Change HP");
        else if (code == 312)
            this->setWindowTitle("Change SP");
        else if (code == 313)
            this->setWindowTitle("Change EXP");
        else if (code == 314)
            this->setWindowTitle("Change Level");
        this->ui->label->setText("Actor:");

        db->fill_combo(this->ui->combo, RPGDB::ACTORS, true, 3);
        this->ui->combo->insertItem(0, "Entire Party");
        this->ui->combo->setCurrentIndex(parameters.at(0).toInt());

        if (parameters.at(1).toInt() == 0) this->ui->radio_increase->setChecked(true);
        else this->ui->radio_decrease->setChecked(true);

        if (parameters.at(2).toInt() == 0) //constant
        {
            this->ui->radio_constant->setChecked(true);
            this->ui->spin_constant->setValue(parameters.at(3).toInt());
        }
        else //variable
        {
            this->ui->radio_variable->setChecked(true);
            this->ui->widget_variable->setValue(parameters.at(3).toInt());
        }

        if (code == 311)
            this->ui->check_battle_knockout->setChecked(parameters.at(4).toBool()); //HP only
        else if (code != 311)
            this->ui->check_battle_knockout->setVisible(false);
    }
    else if (code == 317) //Change Parameters
    {
        this->setWindowTitle("Change Parameters");
        this->ui->label->setText("Actor:");

        db->fill_combo(this->ui->combo, RPGDB::ACTORS, true, 3);
        this->ui->combo->setCurrentIndex(parameters.at(0).toInt()-1); //no entire party entry

        if (parameters.at(2).toInt() == 0) this->ui->radio_increase->setChecked(true);
        else this->ui->radio_decrease->setChecked(true);

        if (parameters.at(3).toInt() == 0) //constant
        {
            this->ui->radio_constant->setChecked(true);
            this->ui->spin_constant->setValue(parameters.at(4).toInt());
        }
        else //variable
        {
            this->ui->radio_variable->setChecked(true);
            this->ui->widget_variable->setValue(parameters.at(4).toInt());
        }
        this->ui->combo_param->setCurrentIndex(parameters.at(1).toInt());

        this->ui->check_battle_knockout->setVisible(false);
    }
    else if (code == 331 || code == 332) //change enemy HP, SP
    {
        this->ui->label_param->setVisible(false);
        this->ui->combo_param->setVisible(false);
        if (code == 331)
            this->setWindowTitle("Change Enemy HP");
        else if (code == 332)
            this->setWindowTitle("Change Enemy SP");

        this->ui->label->setText("Enemy:");

        this->ui->combo->addItem("Entire Troop");
        for (int i = 0; i < 8; i++)
            this->ui->combo->addItem(QString("%1.").arg(i+1));

        this->ui->combo->setCurrentIndex(parameters.at(0).toInt()+1);

        if (parameters.at(1).toInt() == 0) this->ui->radio_increase->setChecked(true);
        else this->ui->radio_decrease->setChecked(true);

        if (parameters.at(2).toInt() == 0) //constant
        {
            this->ui->radio_constant->setChecked(true);
            this->ui->spin_constant->setValue(parameters.at(3).toInt());
        }
        else //variable
        {
            this->ui->radio_variable->setChecked(true);
            this->ui->widget_variable->setValue(parameters.at(3).toInt());
        }

        if (code == 331)
            this->ui->check_battle_knockout->setChecked(parameters.at(4).toBool()); //HP only
        else if (code != 331)
            this->ui->check_battle_knockout->setVisible(false);
    }
}

IncreaseDecreaseDialog::~IncreaseDecreaseDialog()
{
    delete ui;
}

void IncreaseDecreaseDialog::on_button_cancel_clicked()
{
    this->close();
}


void IncreaseDecreaseDialog::on_button_ok_clicked()
{
    if (code == 125) //change gold
    {
        QJsonArray parameters;
        parameters.append(this->ui->radio_increase->isChecked() ? 0 : 1);
        parameters.append(this->ui->radio_constant->isChecked() ? 0 : 1);

        if (this->ui->radio_constant->isChecked())
            parameters.append(this->ui->spin_constant->value());
        else //if (this->ui->radio_variable->isChecked())
            parameters.append(this->ui->widget_variable->getValue());
        emit ok_clicked(parameters);
    }
    else if (code == 126 || code == 127 || code == 128) //change item, weapon, armor
    {
        QJsonArray parameters;
        parameters.append(this->ui->combo->currentIndex()+1);
        parameters.append(this->ui->radio_increase->isChecked() ? 0 : 1);
        parameters.append(this->ui->radio_constant->isChecked() ? 0 : 1);

        if (this->ui->radio_constant->isChecked())
            parameters.append(this->ui->spin_constant->value());
        else //if (this->ui->radio_variable->isChecked())
            parameters.append(this->ui->widget_variable->getValue());
        emit ok_clicked(parameters);
    }
    else if (code == 311 || code == 312 || code == 315 || code == 316) //Change HP, SP, EXP, LEVEL
    {
        QJsonArray parameters;
        parameters.append(this->ui->combo->currentIndex()); //entire party is on place 0
        parameters.append(this->ui->radio_increase->isChecked() ? 0 : 1);
        parameters.append(this->ui->radio_constant->isChecked() ? 0 : 1);

        if (this->ui->radio_constant->isChecked())
            parameters.append(this->ui->spin_constant->value());
        else //if (this->ui->radio_variable->isChecked())
            parameters.append(this->ui->widget_variable->getValue());

        if (code == 311)
            parameters.append(this->ui->check_battle_knockout->isChecked());
        emit ok_clicked(parameters);
    }
    else if (code == 317) //Change Parameters
    {
        QJsonArray parameters;
        parameters.append(this->ui->combo->currentIndex()+1); //no entire party here
        parameters.append(this->ui->combo_param->currentIndex());
        parameters.append(this->ui->radio_increase->isChecked() ? 0 : 1);
        parameters.append(this->ui->radio_constant->isChecked() ? 0 : 1);

        if (this->ui->radio_constant->isChecked())
            parameters.append(this->ui->spin_constant->value());
        else //if (this->ui->radio_variable->isChecked())
            parameters.append(this->ui->widget_variable->getValue());

        emit ok_clicked(parameters);
    }
    else if (code == 331 || code == 332) //Change Enemy HP, SP
    {
        QJsonArray parameters;
        parameters.append(this->ui->combo->currentIndex()-1); //entire troop is defined as -1
        parameters.append(this->ui->radio_increase->isChecked() ? 0 : 1);
        parameters.append(this->ui->radio_constant->isChecked() ? 0 : 1);

        if (this->ui->radio_constant->isChecked())
            parameters.append(this->ui->spin_constant->value());
        else //if (this->ui->radio_variable->isChecked())
            parameters.append(this->ui->widget_variable->getValue());

        if (code == 331)
            parameters.append(this->ui->check_battle_knockout->isChecked());
        emit ok_clicked(parameters);
    }


    this->close();
}

