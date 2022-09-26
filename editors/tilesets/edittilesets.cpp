#include "edittilesets.h"
#include "ui_edittilesets.h"

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

void EditTilesets::set_tileset(int n)
{
    if (this->system->tileset_list.length() <= n) return;
    RPGTileset *current_tileset = this->system->tileset_list.at(n);
}
