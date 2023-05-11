#include "mapconnectiondialog.h"
#include "ui_mapconnectiondialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/rpgmapcontroller.h"

#include "RXIO2/parser.h"
#include "RXIO2/fileopener.h"

MapConnectionDialog::MapConnectionDialog(RPGDB *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapConnectionDialog)
{
    ui->setupUi(this);
    this->db = db;
    QString filepath = FileOpener(db->data_dir,"map_connections.dat").get_existing_file();
    if (filepath.isEmpty())
    {
        QMessageBox::critical(this, "Not supported", "This feature is not supported in your project");
        this->close();
        return;
    }
    Parser parser(&connection_file, &param_oders,filepath);

    //qDebug() << connection_file.array();
    this->ui->graphicsView->setScene(new QGraphicsScene);
    this->ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    this->list_maps();
    this->ui->treeWidget->hideColumn(1);
    this->ui->treeWidget->hideColumn(2);


}

MapConnectionDialog::~MapConnectionDialog()
{
    delete ui;
}

void MapConnectionDialog::list_maps()
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
                non_toplevel_maps.removeAt(i--); //do this at the end

            }
        }
        int cnt2 = non_toplevel_maps.count();
        if (cnt1 == cnt2)
            break; //break here if parent_id loop
    }
    this->ui->treeWidget->sortItems(1,Qt::SortOrder::AscendingOrder);
}

void MapConnectionDialog::display_maps(int id)
{
    this->ui->graphicsView->scene()->clear();
    this->ui->graphicsView->update();

    QImage map = this->render_map(id);
    QImage new_map;
    QGraphicsPixmapItem *selected_map = new QGraphicsPixmapItem(QPixmap::fromImage(map));
    selected_map->setPos(0,0);
    this->ui->graphicsView->scene()->addItem(selected_map);


    QMap<int,QRect> processed_maps;
    processed_maps.insert(id, QRect(0,0,map.width(),map.height()));

    bool changed = false;
    do
    {
        changed = false;
        for (int i = 0; i < connection_file.array().count(); i++)
        {
            QJsonArray conn = connection_file.array().at(i).toArray();
            //TODO conn should have enough entries

            int new_map_id;
            int delta;
            QRect neightbour;
            QString direction;
            if (processed_maps.contains(conn.at(0).toInt()) && !processed_maps.contains(conn.at(3).toInt()))
            {
                direction = conn.at(1).toString();
                new_map_id = conn.at(3).toInt();
                neightbour = processed_maps.value(conn.at(0).toInt());
                delta = 32*(conn.at(2).toInt()-conn.at(5).toInt());
            }
            else if (!processed_maps.contains(conn.at(0).toInt()) && processed_maps.contains(conn.at(3).toInt()))
            {
                direction = conn.at(4).toString();
                new_map_id = conn.at(0).toInt();
                neightbour = processed_maps.value(conn.at(3).toInt());
                delta = 32*(conn.at(5).toInt()-conn.at(2).toInt());
            }
            else continue;
            changed = true;

            //qDebug() << conn << new_map_id << neightbour;

            new_map = this->render_map(new_map_id);
            QGraphicsPixmapItem *new_map_item = new QGraphicsPixmapItem(QPixmap::fromImage(new_map));

            QPoint pos;
            if (direction == "N")
                pos = QPoint(neightbour.left()+delta,neightbour.y()-new_map.height());
            else if (direction == "W")
                pos = QPoint(neightbour.x()-new_map.width(), neightbour.top()+delta);
            else if (direction == "E")
                pos = QPoint(neightbour.right(), neightbour.top()+delta);
            else if (direction == "S")
                pos = QPoint(neightbour.left()+delta,neightbour.bottom());

            new_map_item->setPos(pos);
            processed_maps.insert(new_map_id, QRect(pos,QSize(new_map.width(),new_map.height())));

            this->ui->graphicsView->scene()->addItem(new_map_item);
        }
    }
    while (changed);



    QRect scene_rect(0,0,0,0);
    QMap<int, QRect>::iterator i;
    for (i = processed_maps.begin(); i != processed_maps.end(); ++i)
    {
        //qDebug() << i.value();
        if (i.value().top() < scene_rect.top())
            scene_rect.setTop(i.value().top());
        if (i.value().left() < scene_rect.left())
            scene_rect.setLeft(i.value().left());
        if (i.value().right() > scene_rect.right())
            scene_rect.setRight(i.value().right());
        if (i.value().bottom() > scene_rect.bottom())
            scene_rect.setBottom(i.value().bottom());
    }
    this->ui->graphicsView->scene()->setSceneRect(scene_rect);
    //qDebug() << scene_rect;

    this->ui->graphicsView->centerOn(new_map.width()/2, new_map.height()/2);
}

QImage MapConnectionDialog::render_map(int id)
{
    QJsonObject map = db->get_mapfile_by_id(id)->object();
    if (map.value("RXClass").toString() != "RPG::Map")
        return QImage();

    QJsonObject tileset = db->get_tileset_by_id(map.value("@tileset_id").toInt());
    if (tileset.value("RXClass").toString() != "RPG::Tileset")
        return QImage();

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

    return img;
}

void MapConnectionDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    int id = current->text(2).toInt();

    this->display_maps(id);
}

void MapConnectionDialog::on_button_ok_clicked()
{
    this->close();
}


void MapConnectionDialog::on_button_cancel_clicked()
{
    this->close();
}


void MapConnectionDialog::on_horizontalSlider_valueChanged(int value)
{
    qreal scale = qPow(qreal(2), (value - 250) / qreal(50));

    QTransform matrix;
    matrix.scale(scale,scale);
    this->ui->graphicsView->setTransform(matrix);
}

