#include "mapproperties.h"
#include "ui_mapproperties.h"

MapProperties::MapProperties(RPGMapInfo *info, QHash<int, RPGTileset *> *tilesets, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapProperties)
{
    ui->setupUi(this);
    this->mapinfo = info;
    this->ui->line_id->setText(QString::number(this->mapinfo->id));
    this->ui->line_name->setText(this->mapinfo->name);


    QHashIterator<int, RPGTileset*> i(*tilesets);
    while (i.hasNext()) {
        i.next();
        this->ui->combo_tileset->addItem(i.value()->name, QVariant(i.key()));
    }


    this->ui->spin_x->setValue(info->map->width);
    this->ui->spin_y->setValue(info->map->height);

    //this->ui->combo_tileset;
}

MapProperties::~MapProperties()
{
    qDebug() << "dialog deleted";
    delete ui;
}

void MapProperties::closeEvent(QCloseEvent *event)
{
    this->deleteLater();
}
