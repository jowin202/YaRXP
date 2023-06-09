#include "trainerpokemon.h"
#include "ui_trainerpokemon.h"

TrainerPokemon::TrainerPokemon(QJsonObject obj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainerPokemon)
{
    ui->setupUi(this);

    QJsonArray dict_list = obj.value("dict_list").toArray();
    //qDebug() << dict_list;
    for (int i = 0; i < dict_list.count(); i++)
    {
        QJsonArray element = dict_list.at(i).toArray();
        if (element.at(0).toObject().value("symbol_value") == "species")
            this->ui->line_species->setText(element.at(1).toObject().value("symbol_value").toString());
        else if (element.at(0).toObject().value("symbol_value") == "level")
            this->ui->spin_level->setValue(element.at(1).toInt());
        else if (element.at(0).toObject().value("symbol_value") == "name")
            this->ui->line_name->setText(element.at(1).toString());
        else if (element.at(0).toObject().value("symbol_value") == "item")
            this->ui->line_item->setText(element.at(1).toObject().value("symbol_value").toString());
        else if (element.at(0).toObject().value("symbol_value") == "gender")
            this->ui->combo_gender->setCurrentIndex(element.at(1).toInt());
        else if (element.at(0).toObject().value("symbol_value") == "shininess")
            this->ui->check_shiny->setChecked(element.at(1).toBool());
        else if (element.at(0).toObject().value("symbol_value") == "super_shininess")
            this->ui->check_super_shiny->setChecked(element.at(1).toBool());
        else if (element.at(0).toObject().value("symbol_value") == "shadowness")
            this->ui->check_shadow->setChecked(element.at(1).toBool());
        else if (element.at(0).toObject().value("symbol_value") == "moves")
        {
            QJsonArray move_list = element.at(1).toArray();
            if (move_list.count() >= 1)
                this->ui->line_move_1->setText(move_list.at(0).toObject().value("symbol_value").toString());
            if (move_list.count() >= 2)
                this->ui->line_move_2->setText(move_list.at(1).toObject().value("symbol_value").toString());
            if (move_list.count() >= 3)
                this->ui->line_move_3->setText(move_list.at(2).toObject().value("symbol_value").toString());
            if (move_list.count() == 4)
                this->ui->line_move_4->setText(move_list.at(3).toObject().value("symbol_value").toString());
        }
        //else
            //qDebug() << element.at(0).toObject().value("symbol_value");

    }
}

TrainerPokemon::~TrainerPokemon()
{
    delete ui;
}
