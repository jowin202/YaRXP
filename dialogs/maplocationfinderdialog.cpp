#include "maplocationfinderdialog.h"
#include "ui_maplocationfinderdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/rpgmapcontroller.h"

MapLocationFinderDialog::MapLocationFinderDialog(RPGDB *db, int id, int x, int y, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapLocationFinderDialog)
{
    ui->setupUi(this);
    this->db = db;

    this->x = x;
    this->y = y;
    this->id = id;

    this->list_maps();
    this->ui->treeWidget->hideColumn(1);
    this->ui->treeWidget->hideColumn(2);

    connect(this->ui->graphicsView, &MapLocationArea::pressed, [=](int x, int y) { this->x = x; this->y = y;});
}

MapLocationFinderDialog::~MapLocationFinderDialog()
{
    delete ui;
}

void MapLocationFinderDialog::list_maps()
{
    RPGMapInfoController mic = RPGMapInfoController(db);
    QJsonArray toplevel_maps = mic.get_toplevel_maps();
    QJsonArray non_toplevel_maps = mic.get_child_maps();

    for (int i = 0; i < toplevel_maps.count(); i++)
    {
        QStringList columns;
        columns << toplevel_maps.at(i).toObject().value("@name").toString();
        columns << QString::number(toplevel_maps.at(i).toObject().value("@order").toInt()).rightJustified(3,'0');
        columns << QString::number(toplevel_maps.at(i).toObject().value("id").toInt());
        QTreeWidgetItem *item = new QTreeWidgetItem(columns);
        this->ui->treeWidget->addTopLevelItem(item);
        this->id_map.insert(toplevel_maps.at(i).toObject().value("id").toInt(), item);
        this->ui->treeWidget->expandItem(item);
        if (this->id == toplevel_maps.at(i).toObject().value("id").toInt())
        {
            this->ui->treeWidget->setCurrentItem(item);
            this->ui->treeWidget->scrollToItem(item);
            this->on_treeWidget_currentItemChanged(item, 0);
        }
    }


    while(non_toplevel_maps.count() > 0)
    {
        int cnt1 = non_toplevel_maps.count();
        for (int i = 0; i < non_toplevel_maps.count(); i++)
        {
            if (this->id_map.contains(non_toplevel_maps.at(i).toObject().value("@parent_id").toInt()))
            {
                QStringList columns;
                columns << non_toplevel_maps.at(i).toObject().value("@name").toString();
                columns << QString::number(non_toplevel_maps.at(i).toObject().value("@order").toInt()).rightJustified(3,'0');
                columns << QString::number(non_toplevel_maps.at(i).toObject().value("id").toInt());
                QTreeWidgetItem *item = new QTreeWidgetItem(columns);
                this->id_map.value(non_toplevel_maps.at(i).toObject().value("@parent_id").toInt())->addChild(item);
                this->id_map.insert(non_toplevel_maps.at(i).toObject().value("id").toInt(), item);
                this->ui->treeWidget->expandItem(item);
                if (this->id == non_toplevel_maps.at(i).toObject().value("id").toInt())
                {
                    this->ui->treeWidget->setCurrentItem(item);
                    this->ui->treeWidget->scrollToItem(item);
                    this->on_treeWidget_currentItemChanged(item, 0);
                }
                non_toplevel_maps.removeAt(i--); //do this at the end

            }
        }
        int cnt2 = non_toplevel_maps.count();
        if (cnt1 == cnt2)
            break; //break here if parent_id loop
    }
    this->ui->treeWidget->sortItems(1,Qt::SortOrder::AscendingOrder);
}



void MapLocationFinderDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    this->id = current->text(2).toInt();

    QJsonObject map = db->get_mapfile_by_id(id)->object();
    if (map.value("RXClass").toString() != "RPG::Map")
        return;

    QJsonObject tileset = db->get_tileset_by_id(map.value("@tileset_id").toInt());

    if (tileset.value("RXClass").toString() != "RPG::Tileset")
        return;


    RPGMapController mc;
    mc.setDB(db);
    mc.setMap(id);

    QJsonObject map_data = map.value("@data").toObject();
    int width = map_data.value("x").toInt();
    int height = map_data.value("y").toInt();

    QImage img(width*32,height*32,QImage::Format_ARGB32_Premultiplied);
    QPainter painter;
    painter.begin(&img);
    painter.fillRect(0,0,img.width(), img.height(), Qt::black);
    for (int z = 0; z < 3; z++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int pos = x + y * width + height*width * z;
                painter.drawImage(32*x,32*y,mc.get_tile_from_id(map_data.value("values").toArray().at(pos).toInt()));
            }
        }
    }
    painter.end();

    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    background->setPos(0,0);
    this->ui->graphicsView->scene()->clear();
    this->ui->graphicsView->scene()->addItem(background);
    this->ui->graphicsView->scene()->setSceneRect(0,0,32*width,32*height);
    this->ui->graphicsView->define_rectangle(x,y);
}

void MapLocationFinderDialog::on_button_ok_clicked()
{
    emit ok_clicked(id,x,y);
    this->close();
}


void MapLocationFinderDialog::on_button_cancel_clicked()
{
    this->close();
}

