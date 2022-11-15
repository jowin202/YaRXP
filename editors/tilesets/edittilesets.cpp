#include "edittilesets.h"
#include "ui_edittilesets.h"

#include "dialogs/imagedialog.h"

#include "tile.h"
#include "RXIO/RXObjects/rpgsystem.h"

EditTilesets::EditTilesets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditTilesets)
{
    ui->setupUi(this);

}

EditTilesets::~EditTilesets()
{
    delete ui;
}

void EditTilesets::setSystem(RPGSystem *system)
{
    this->system = system;
    this->ui->tileset_widget->setSystem(system);
}

void EditTilesets::set_tileset(int n)
{
    if (this->system->tileset_list.length() <= n) return;
    RPGTileset *current_tileset = this->system->tileset_list.at(n);
    this->current = n;

    this->ui->line_name->setText(current_tileset->name);
    this->ui->line_tileset_graphic->setText(current_tileset->tileset_name);
    this->ui->line_autotile_1->setText(current_tileset->autotile_names.at(0));
    this->ui->line_autotile_2->setText(current_tileset->autotile_names.at(1));
    this->ui->line_autotile_3->setText(current_tileset->autotile_names.at(2));
    this->ui->line_autotile_4->setText(current_tileset->autotile_names.at(3));
    this->ui->line_autotile_5->setText(current_tileset->autotile_names.at(4));
    this->ui->line_autotile_6->setText(current_tileset->autotile_names.at(5));
    this->ui->line_autotile_7->setText(current_tileset->autotile_names.at(6));


    this->ui->line_panorama->setText(current_tileset->panorama_name);
    this->panorama_hue = current_tileset->panorama_hue;

    this->ui->line_fog_graphic->setText(current_tileset->fog_name);
    this->fog_sx = current_tileset->fog_sx;
    this->fog_sy = current_tileset->fog_sy;
    this->fog_hue = current_tileset->fog_hue;
    this->fog_opacity = current_tileset->fog_opacity;
    this->fog_zoom = current_tileset->fog_zoom;
    this->fog_blend_type = current_tileset->fog_blend_type;

    this->ui->line_battleback->setText(current_tileset->battleback_name);

    this->ui->tileset_widget->set_tileset(this->ui->line_tileset_graphic->text());
    this->ui->tileset_widget->set_autotile_1(this->ui->line_autotile_1->text());
    this->ui->tileset_widget->set_autotile_2(this->ui->line_autotile_2->text());
    this->ui->tileset_widget->set_autotile_3(this->ui->line_autotile_3->text());
    this->ui->tileset_widget->set_autotile_4(this->ui->line_autotile_4->text());
    this->ui->tileset_widget->set_autotile_5(this->ui->line_autotile_5->text());
    this->ui->tileset_widget->set_autotile_6(this->ui->line_autotile_6->text());
    this->ui->tileset_widget->set_autotile_7(this->ui->line_autotile_7->text());

    this->ui->tileset_widget->set_data(&current_tileset->passages, &current_tileset->priorities, &current_tileset->terrain_tag);


}

void EditTilesets::save()
{
    int n = this->current;
    if (this->system->tileset_list.length() <= n) return;
    RPGTileset *current_tileset = this->system->tileset_list.at(n);

    current_tileset->name = this->ui->line_name->text();
    current_tileset->tileset_name = this->ui->line_tileset_graphic->text();

    current_tileset->autotile_names.clear();
    current_tileset->autotile_names << this->ui->line_autotile_1->text();
    current_tileset->autotile_names << this->ui->line_autotile_2->text();
    current_tileset->autotile_names << this->ui->line_autotile_3->text();
    current_tileset->autotile_names << this->ui->line_autotile_4->text();
    current_tileset->autotile_names << this->ui->line_autotile_5->text();
    current_tileset->autotile_names << this->ui->line_autotile_6->text();
    current_tileset->autotile_names << this->ui->line_autotile_7->text();

    current_tileset->panorama_name = this->ui->line_panorama->text();
    current_tileset->panorama_hue = this->panorama_hue;


    current_tileset->fog_name = this->ui->line_fog_graphic->text();
    current_tileset->fog_sx = this->fog_sx;
    current_tileset->fog_sy = this->fog_sy;
    current_tileset->fog_hue = this->fog_hue;
    current_tileset->fog_opacity = this->fog_opacity;
    current_tileset->fog_zoom = this->fog_zoom;
    current_tileset->fog_blend_type = this->fog_blend_type;


    current_tileset->battleback_name = this->ui->line_battleback->text();
}

void EditTilesets::on_button_choose_tileset_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::TILESETS, this->ui->line_tileset_graphic->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_tileset_graphic, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_tileset(QString)));
}

void EditTilesets::on_button_autotile_1_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::AUTOTILES, this->ui->line_autotile_1->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_1, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_autotile_1(QString)));
}

void EditTilesets::on_button_autotile_2_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::AUTOTILES, this->ui->line_autotile_2->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_2, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_autotile_2(QString)));
}

void EditTilesets::on_button_autotile_3_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::AUTOTILES, this->ui->line_autotile_3->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_3, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_autotile_3(QString)));
}

void EditTilesets::on_button_autotile_4_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::AUTOTILES, this->ui->line_autotile_4->text());
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_autotile_4(QString)));
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_4, SLOT(setText(QString)));
}

void EditTilesets::on_button_autotile_5_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::AUTOTILES, this->ui->line_autotile_5->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_5, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_autotile_5(QString)));
}

void EditTilesets::on_button_autotile_6_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::AUTOTILES, this->ui->line_autotile_6->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_6, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_autotile_6(QString)));
}

void EditTilesets::on_button_autotile_7_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::AUTOTILES, this->ui->line_autotile_7->text());
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_autotile_7, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->tileset_widget, SLOT(set_autotile_7(QString)));
}

void EditTilesets::on_button_panorama_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::PANOMRAMAS, this->ui->line_panorama->text());
    dialog->set_hue(this->panorama_hue);
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_panorama, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked_with_hue(int)), this, SLOT(set_panorama_hue(int)));
}

void EditTilesets::on_button_fog_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::FOGS, this->ui->line_fog_graphic->text());
    dialog->set_hue(this->fog_hue);
    dialog->set_fog_options(fog_sx, fog_sy, fog_opacity, fog_zoom, fog_blend_type);
    dialog->show();
    connect(dialog, SIGNAL(ok_clicked(QString)), this->ui->line_fog_graphic, SLOT(setText(QString)));
    connect(dialog, SIGNAL(ok_clicked_with_hue(int)), this, SLOT(set_fog_hue(int)));
    connect(dialog, SIGNAL(ok_clicked_with_fog_options(int,int,int,int,int)), this, SLOT(set_fog_options(int,int,int,int,int)));
}

void EditTilesets::on_button_battleback_clicked()
{
    ImageDialog *dialog = new ImageDialog(system, ImageDialog::BATTLEBACKS, this->ui->line_battleback->text());
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

