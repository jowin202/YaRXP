#include "edittilesets.h"
#include "ui_edittilesets.h"

#include "dialogs/imagedialog.h"

#include "tile.h"
#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"

EditTilesets::EditTilesets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditTilesets)
{
    ui->setupUi(this);
    connect(this->ui->line_autotile_1, SIGNAL(textChanged(QString)), this, SLOT(save_changed_autotile_names()));
    connect(this->ui->line_autotile_2, SIGNAL(textChanged(QString)), this, SLOT(save_changed_autotile_names()));
    connect(this->ui->line_autotile_3, SIGNAL(textChanged(QString)), this, SLOT(save_changed_autotile_names()));
    connect(this->ui->line_autotile_4, SIGNAL(textChanged(QString)), this, SLOT(save_changed_autotile_names()));
    connect(this->ui->line_autotile_5, SIGNAL(textChanged(QString)), this, SLOT(save_changed_autotile_names()));
    connect(this->ui->line_autotile_6, SIGNAL(textChanged(QString)), this, SLOT(save_changed_autotile_names()));
    connect(this->ui->line_autotile_7, SIGNAL(textChanged(QString)), this, SLOT(save_changed_autotile_names()));
}

EditTilesets::~EditTilesets()
{
    delete ui;
}

void EditTilesets::setEC(RPGEditorController *ec)
{
    this->ec = ec;
    this->ec->connect_string_to_text_field(RPGDB::TILESETS, "@name", this->ui->line_name);
    this->ec->connect_string_to_text_field(RPGDB::TILESETS, "@tileset_name", this->ui->line_tileset_name);
    this->ui->tileset_widget->setEC(ec);
    this->ui->tileset_widget->set_mode(Tile::PASSAGES); //do this otherwise segmentation fault

    connect(this->ec, SIGNAL(current_tileset_changed()), this, SLOT(fill_autotile_names()));
    connect(this->ec, SIGNAL(current_tileset_changed()), this->ui->tileset_widget, SLOT(update_tileset()));

    this->ec->connect_string_to_text_field(RPGDB::TILESETS, "@panorama_name", this->ui->line_panorama);
    this->ec->connect_string_to_text_field(RPGDB::TILESETS, "@fog_name", this->ui->line_fog_graphic);
    this->ec->connect_string_to_text_field(RPGDB::TILESETS, "@battleback_name", this->ui->line_battleback);
}


void EditTilesets::on_button_choose_tileset_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::TILESETS, this->ui->line_tileset_name->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_tileset_name, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_tileset(QString)));

}

void EditTilesets::fill_autotile_names()
{
    QJsonArray autotile_names = this->ec->obj_get_jsonvalue(RPGDB::TILESETS, "@autotile_names").toArray();
    this->ui->line_autotile_1->setText(autotile_names.at(0).toString());
    this->ui->line_autotile_2->setText(autotile_names.at(1).toString());
    this->ui->line_autotile_3->setText(autotile_names.at(2).toString());
    this->ui->line_autotile_4->setText(autotile_names.at(3).toString());
    this->ui->line_autotile_5->setText(autotile_names.at(4).toString());
    this->ui->line_autotile_6->setText(autotile_names.at(5).toString());
    this->ui->line_autotile_7->setText(autotile_names.at(6).toString());
}

void EditTilesets::save_changed_autotile_names()
{
    QJsonArray autotile_names;
    autotile_names.append(this->ui->line_autotile_1->text());
    autotile_names.append(this->ui->line_autotile_2->text());
    autotile_names.append(this->ui->line_autotile_3->text());
    autotile_names.append(this->ui->line_autotile_4->text());
    autotile_names.append(this->ui->line_autotile_5->text());
    autotile_names.append(this->ui->line_autotile_6->text());
    autotile_names.append(this->ui->line_autotile_7->text());
    this->ec->obj_set_jsonvalue(RPGDB::TILESETS, "@autotile_names", autotile_names);
}

void EditTilesets::on_button_autotile_1_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::AUTOTILES, this->ui->line_autotile_1->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_1, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(update_tileset()));
}

void EditTilesets::on_button_autotile_2_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::AUTOTILES, this->ui->line_autotile_2->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_2, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(update_tileset()));
}

void EditTilesets::on_button_autotile_3_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::AUTOTILES, this->ui->line_autotile_3->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_3, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(update_tileset()));
}

void EditTilesets::on_button_autotile_4_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::AUTOTILES, this->ui->line_autotile_4->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_4, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(update_tileset()));
}

void EditTilesets::on_button_autotile_5_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::AUTOTILES, this->ui->line_autotile_5->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_5, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(update_tileset()));
}

void EditTilesets::on_button_autotile_6_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::AUTOTILES, this->ui->line_autotile_6->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_6, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(update_tileset()));
}

void EditTilesets::on_button_autotile_7_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::AUTOTILES, this->ui->line_autotile_7->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_7, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(update_tileset()));
}

void EditTilesets::on_button_panorama_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::PANOMRAMAS, this->ui->line_panorama->text());
    dialog->set_hue(this->ec->obj_get_jsonvalue(RPGDB::TILESETS, "@panorama_hue").toInt());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_panorama, SLOT(setText(QString)));
    connect(dialog, &ImageDialog::ok_clicked_with_hue, this, [=](int hue) { this->ec->obj_set_jsonvalue(RPGDB::TILESETS, "@panorama_hue",hue);});
}

void EditTilesets::on_button_fog_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::FOGS, this->ui->line_fog_graphic->text());
    dialog->set_hue(this->ec->obj_get_jsonvalue(RPGDB::TILESETS, "@fog_hue").toInt());
    dialog->set_fog_options(
                this->ec->obj_get_jsonvalue(RPGDB::TILESETS, "@fog_sx").toInt(),
                this->ec->obj_get_jsonvalue(RPGDB::TILESETS, "@fog_sy").toInt(),
                this->ec->obj_get_jsonvalue(RPGDB::TILESETS, "@fog_opacity").toInt(),
                this->ec->obj_get_jsonvalue(RPGDB::TILESETS, "@fog_zoom").toInt(),
                this->ec->obj_get_jsonvalue(RPGDB::TILESETS, "@fog_blend_type").toInt()
                );

    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_fog_graphic, SLOT(setText(QString)));
    connect(dialog, &ImageDialog::ok_clicked_with_hue, this, [=](int hue) { this->ec->obj_set_jsonvalue(RPGDB::TILESETS, "@fog_hue",hue);});
    connect(dialog, &ImageDialog::ok_clicked_with_fog_options, this, [=](int fog_sx, int fog_sy, int fog_opacity, int fog_zoom, int fog_blend_type){
        this->ec->obj_set_jsonvalue(RPGDB::TILESETS, "@fog_sx", fog_sx);
        this->ec->obj_set_jsonvalue(RPGDB::TILESETS, "@fog_sy", fog_sy);
        this->ec->obj_set_jsonvalue(RPGDB::TILESETS, "@fog_opacity", fog_opacity);
        this->ec->obj_set_jsonvalue(RPGDB::TILESETS, "@fog_zoom", fog_zoom);
        this->ec->obj_set_jsonvalue(RPGDB::TILESETS, "@fog_blend_type", fog_blend_type);
    });
}

void EditTilesets::on_button_battleback_clicked()
{
    ImageDialog *dialog = new ImageDialog(ec->get_db(), ImageDialog::BATTLEBACKS, this->ui->line_battleback->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_battleback, SLOT(setText(QString)));
}

void EditTilesets::on_button_passage_clicked()
{
    this->ui->tileset_widget->set_mode(Tile::PASSAGES);
}

void EditTilesets::on_button_passage_4dir_clicked()
{
    this->ui->tileset_widget->set_mode(Tile::PASSAGES4);
}

void EditTilesets::on_button_priority_clicked()
{
    this->ui->tileset_widget->set_mode(Tile::PRIORITY);
}

void EditTilesets::on_button_bush_flag_clicked()
{
    this->ui->tileset_widget->set_mode(Tile::BUSH);
}

void EditTilesets::on_button_counter_flag_clicked()
{
    this->ui->tileset_widget->set_mode(Tile::COUNTER);
}

void EditTilesets::on_button_terrain_flag_clicked()
{
    this->ui->tileset_widget->set_mode(Tile::TERRAIN);
}

