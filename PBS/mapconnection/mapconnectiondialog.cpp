#include "mapconnectiondialog.h"
#include "ui_mapconnectiondialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgmapinfocontroller.h"
#include "RXIO2/rpgmapcontroller.h"

#include "RXIO2/parser.h"
#include "RXIO2/fileopener.h"

#include "mapgraphicsitem.h"

MapConnectionDialog::MapConnectionDialog(RPGDB *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapConnectionDialog)
{
    ui->setupUi(this);
    this->db = db;
    QString filepath_connections = FileOpener(db->data_dir,"map_connections.dat").get_existing_file();
    if (filepath_connections.isEmpty())
    {
        QMessageBox::critical(this, "Not supported", "This feature is not supported in your project");
        this->close();
        return;
    }
    Parser parser(&connection_file, filepath_connections, false, true);


    QString filepath_encounters = FileOpener(db->data_dir,"encounters.dat").get_existing_file();
    if (!filepath_encounters.isEmpty())
    {
        Parser parser(&encounters_file, filepath_encounters, false, true);
        QFile f("/tmp/test.json");
        f.open(QIODevice::WriteOnly);
        f.write(encounters_file.toJson());
        f.close();
    }


    this->ui->graphicsView->setScene(new QGraphicsScene);
    this->ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    this->ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    connect(this->ui->graphicsView, &ConnectedMapView::zoom_in, [=](){ this->ui->horizontalSlider->setValue(this->ui->horizontalSlider->value()+10);});
    connect(this->ui->graphicsView, &ConnectedMapView::zoom_out, [=](){ this->ui->horizontalSlider->setValue(this->ui->horizontalSlider->value()-10);});

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

void MapConnectionDialog::display_maps(int id, bool center)
{
    this->ui->graphicsView->scene()->clear();
    //this->ui->graphicsView->update();

    MapGraphicsItem *new_map = 0;
    MapGraphicsItem *selected_map = this->render_map(id);
    if (selected_map == 0) return;
    selected_map->setPos(0,0);
    this->ui->graphicsView->scene()->addItem(selected_map);

    QMap<int,QRect> processed_maps;
    processed_maps.insert(id, QRect(0,0,selected_map->width(),selected_map->height()));

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

            QPoint pos;
            if (direction == "N")
                pos = QPoint(neightbour.left()+delta,neightbour.y()-new_map->height());
            else if (direction == "W")
                pos = QPoint(neightbour.x()-new_map->width(), neightbour.top()+delta);
            else if (direction == "E")
                pos = QPoint(neightbour.right(), neightbour.top()+delta);
            else if (direction == "S")
                pos = QPoint(neightbour.left()+delta,neightbour.bottom());

            new_map->setPos(pos);
            processed_maps.insert(new_map_id, QRect(pos,QSize(new_map->width(),new_map->height())));

            this->ui->graphicsView->scene()->addItem(new_map);
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
    this->ui->graphicsView->scene()->update();
    //shitty workaround to center everything
    if (center)
    {
        QTimer::singleShot(1, [=](){
            this->ui->graphicsView->centerOn(selected_map->width()/2, selected_map->height()/2);
        });
    }
}

MapGraphicsItem *MapConnectionDialog::render_map(int id)
{
    QJsonObject map = db->get_mapfile_by_id(id)->object();
    if (map.value("RXClass").toString() != "RPG::Map")
        return 0;

    QJsonObject tileset = db->get_tileset_by_id(map.value("@tileset_id").toInt());
    if (tileset.value("RXClass").toString() != "RPG::Tileset")
        return 0;

    MapGraphicsItem *result = new MapGraphicsItem();
    QImage img;

    if (cached_maps.contains(id))
    {
        img = cached_maps.value(id).img;
        for (int i = 0; i < cached_maps.value(id).links.count(); i++)
            result->addLink(cached_maps.value(id).links.at(i).rect,cached_maps.value(id).links.at(i).dest);
    }
    else
    {
        cached_map current_map;
        RPGMapController mc;
        mc.setDB(db);
        mc.setMap(id);

        QJsonObject map_events = map.value("@events").toObject();
        QJsonObject map_data = map.value("@data").toObject();
        int width = map_data.value("x").toInt();
        int height = map_data.value("y").toInt();

        img = QImage(width*32,height*32,QImage::Format_ARGB32_Premultiplied);
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

        //clickable links
        painter.setOpacity(0.5);
        foreach (const QString &key, map_events.keys()) {
            if (key == "RXClass") continue;
            QJsonObject event = map_events.value(key).toObject();
            QJsonArray pages = event.value("@pages").toArray();
            bool has_teleport = false;
            for (int p = 0; p < pages.count(); p++)
            {
                QJsonArray event_list = pages.at(p).toObject().value("@list").toArray();
                for (int e = 0; e < event_list.count(); e++)
                {
                    if (event_list.at(e).toObject().value("@code").toInt() == 201)
                    {
                        has_teleport = true;
                        result->addLink(QRect(32*event.value("@x").toInt(),32*event.value("@y").toInt(), 32,32),
                                        event_list.at(e).toObject().value("@parameters").toArray().at(1).toInt());
                        link l;
                        l.dest = event_list.at(e).toObject().value("@parameters").toArray().at(1).toInt();
                        l.rect = QRect(32*event.value("@x").toInt(),32*event.value("@y").toInt(), 32,32);
                        current_map.links.append(l);
                    }
                }
            }
            if (has_teleport)
            {
                painter.fillRect(32*event.value("@x").toInt(),32*event.value("@y").toInt(), 32,32, Qt::blue);
            }
        }
        painter.end();
        current_map.img = img;
        cached_maps.insert(id, current_map);
    }


    result->setImage(img, id);

    connect(result, SIGNAL(mouse_clicked(int)), this, SLOT(jump_to_item(int)));
    return result;
}


void MapConnectionDialog::jump_to_item(int val)
{
    QList<QTreeWidgetItem*> items = this->ui->treeWidget->findItems(QString::number(val),Qt::MatchExactly|Qt::MatchRecursive,2);
    if (items.length() > 0)
    {
        this->ui->treeWidget->setCurrentItem(items.at(0));
    }
}

void MapConnectionDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    this->current_id = current->text(2).toInt();
    this->display_maps(this->current_id);
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


void MapConnectionDialog::on_button_refresh_clicked()
{
    this->cached_maps.clear();
    if (this->current_id > 0)
        this->display_maps(this->current_id, false); //dont center view
}

