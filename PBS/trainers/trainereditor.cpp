#include "trainereditor.h"
#include "ui_trainereditor.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/writer.h"
#include "RXIO2/parser.h"
#include "RXIO2/fileopener.h"

#include "../pbsfactory.h"

#include "trainerpokemon.h"
#include "trainertype.h"


TrainerEditor::TrainerEditor(RPGDB * db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TrainerEditor)
{
    ui->setupUi(this);
    this->db = db;

    QString filepath_trainer = FileOpener(db->data_dir,"trainers.dat").get_existing_file();
    if (filepath_trainer.isEmpty())
    {
        QMessageBox::critical(this, "Not supported", "This feature is not supported in your project");
        this->close();
        return;
    }
    Parser parser(&trainers_file, filepath_trainer, false, true);

    QString filepath_trainer_types = FileOpener(db->data_dir,"trainer_types.dat").get_existing_file();
    if (filepath_trainer.isEmpty())
    {
        QMessageBox::critical(this, "Not supported", "This feature is not supported in your project");
        this->close();
        return;
    }
    Parser parser2(&trainer_types_file, filepath_trainer_types, false, true);

    this->read_trainers();
}

TrainerEditor::~TrainerEditor()
{
    delete ui;
}

void TrainerEditor::read_trainers()
{
    this->ui->list_trainers->clear();
    QJsonArray dict_list = this->trainers_file.object().value("dict_list").toArray();
    for (int i = 0; i < dict_list.count(); i++)
    {
        this->ui->list_trainers->addItem(dict_list.at(i).toArray().at(0).toArray().at(0).toObject().value("symbol_value").toString() + " " +
                                         dict_list.at(i).toArray().at(0).toArray().at(1).toString() +
                                         (dict_list.at(i).toArray().at(0).toArray().at(2).toInt(0) != 0 ?
                    QString(" (v %1)").arg(dict_list.at(i).toArray().at(0).toArray().at(2).toInt(0)) : ""));
    }

    this->ui->trainer_type_combo->clear();
    QJsonArray trainer_type_dict_list = this->trainer_types_file.object().value("dict_list").toArray();
    for (int i = 0; i < trainer_type_dict_list.count(); i++)
        this->ui->trainer_type_combo->addItem(trainer_type_dict_list.at(i).toArray().at(0).toObject().value("symbol_value").toString(), i);
}

void TrainerEditor::on_list_trainers_currentRowChanged(int currentRow)
{
    QJsonArray current = this->trainers_file.object().value("dict_list").toArray().at(currentRow).toArray();
    //qDebug() << current;
    this->ui->spin_version->setValue(current.at(1).toObject().value("@version").toInt());
    this->ui->line_name->setText(current.at(1).toObject().value("@real_name").toString());
    this->ui->line_lose_text->setText(current.at(1).toObject().value("@real_lose_text").toString());

    //items
    this->ui->list_trainer_items->clear();
    QJsonArray items = current.at(1).toObject().value("@items").toArray();
    for (int i = 0; i < items.count(); i++)
        this->ui->list_trainer_items->addItem(items.at(i).toObject().value("symbol_value").toString());

    //trainer type
    for (int i = 0; i < this->ui->trainer_type_combo->count(); i++)
        if (this->ui->trainer_type_combo->itemText(i) == current.at(1).toObject().value("@trainer_type").toObject().value("symbol_value").toString())
            this->ui->trainer_type_combo->setCurrentIndex(i);

    //sprites
    QString trainer_dir = FileOpener(db->graphics_dir, "Trainers").get_existing_directory();

    QImage overworld = FileOpener(db->character_dir, QString("trainer_%1").arg(current.at(1).toObject().value("@trainer_type").toObject().value("symbol_value").toString())).get_image();
    this->ui->label_sprite_overworld->setPixmap(QPixmap::fromImage(overworld));

    QImage front = FileOpener(trainer_dir, QString("%1").arg(current.at(1).toObject().value("@trainer_type").toObject().value("symbol_value").toString())).get_image();
    this->ui->label_sprite_front->setPixmap(QPixmap::fromImage(front));

    //TODO: back sprite

    //pokemon list
    QLayoutItem *item;
    while ( (item = this->ui->layout_pokemon->takeAt(0)) != 0)
        delete item->widget();
    QJsonArray pokemon = current.at(1).toObject().value("@pokemon").toArray();
    for (int i = 0; i < pokemon.count(); i++)
    {
        this->ui->layout_pokemon->addWidget(new TrainerPokemon(pokemon.at(i).toObject()));
    }

}

void TrainerEditor::on_button_change_type_clicked()
{
    int current_type_index = this->ui->trainer_type_combo->currentData().toInt();
    TrainerType *type_win = new TrainerType(trainer_types_file.object().value("dict_list").toArray().at(current_type_index).toArray());
    type_win->show();

    connect(type_win, &TrainerType::finished, [=](QJsonArray result)
    {
        QJsonObject obj = trainer_types_file.object();
        QJsonArray dict_list = obj.value("dict_list").toArray();
        dict_list.removeAt(current_type_index);
        dict_list.insert(current_type_index, result);
        obj.insert("dict_list", dict_list);
        trainer_types_file.setObject(obj);

        /*
        //no reset when trainer ID is not changeable
        int current_trainer = this->ui->list_trainers->currentRow();
        this->read_trainers();
        this->ui->list_trainers->setCurrentRow(current_trainer);
        */
    });
}

