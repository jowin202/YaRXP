#include "importdialog.h"
#include "ui_importdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"
#include "RXIO2/rpgmapcontroller.h"
#include "RXIO2/rpgmapinfocontroller.h"

ImportDialog::ImportDialog(RPGDB *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    this->db = db;
    this->db->fill_combo(this->ui->combo_tileset,RPGDB::TILESETS, true, 3);
    this->ui->combo_tileset->setCurrentIndex(0);
    connect(this->ui->combo_tileset, &QComboBox::currentIndexChanged, [=](){ this->display_map(); });

    this->ui->treeWidget->hideColumn(1);
    this->ui->treeWidget->hideColumn(2);
}

ImportDialog::~ImportDialog()
{
    delete ui;
}

void ImportDialog::list_maps()
{
    RPGMapInfoController mic = RPGMapInfoController(secondary_db);
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

void ImportDialog::display_map()
{
    QJsonObject map = secondary_db->get_mapfile_by_id(id)->object();
    if (map.value("RXClass").toString() != "RPG::Map")
        return;

    QJsonObject tileset_orig = secondary_db->get_tileset_by_id(map.value("@tileset_id").toInt());
    QJsonObject tileset_new = db->get_tileset_by_id(this->ui->combo_tileset->currentData().toInt());

    if (tileset_orig.value("RXClass").toString() != "RPG::Tileset" && tileset_new.value("RXClass").toString() != "RPG::Tileset")
        return;

    QImage tileset_img = FileOpener(db->tileset_dir,tileset_new.value("@tileset_name").toString()).get_image();

    RPGMapController mc;
    mc.setDB(secondary_db);
    mc.setMap(id);


    QJsonObject map_data = map.value("@data").toObject();
    int width = map_data.value("x").toInt();
    int height = map_data.value("y").toInt();

    QImage img_orig(width*32,height*32,QImage::Format_ARGB32_Premultiplied);
    QImage img_new(width*32,height*32,QImage::Format_ARGB32_Premultiplied);
    QPainter painter_orig;
    QPainter painter_new;
    painter_orig.begin(&img_orig);
    painter_new.begin(&img_new);
    painter_orig.fillRect(0,0,img_orig.width(), img_orig.height(), Qt::black);
    painter_new.fillRect(0,0,img_new.width(), img_new.height(), Qt::black);
    for (int z = 0; z < 3; z++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int pos = x + y * width + height*width * z;
                int id = map_data.value("values").toArray().at(pos).toInt() - 0x0180;
                painter_orig.drawImage(32*x,32*y,mc.get_tile_from_id(map_data.value("values").toArray().at(pos).toInt()));
                if (id > 0)
                    painter_new.drawImage(32*x,32*y,tileset_img.copy(32*(id % 8), 32*(id / 8), 32, 32));
            }
        }
    }
    painter_orig.end();
    painter_new.end();

    this->ui->label_before->setPixmap(QPixmap::fromImage(img_orig));
    this->ui->label_after->setPixmap(QPixmap::fromImage(img_new));
}

void ImportDialog::on_button_browse_clicked()
{
    QString name = QFileDialog::getOpenFileName(this, "Choose Project", QDir::homePath(), "RPG Maker Project Files (*.rxproj);;Data Files(*.rxdata)");
    if (name == "")
        return;

    QFileInfo fi(name);

    if (this->secondary_db != 0)
        delete this->secondary_db;

    this->secondary_db = new RPGDB;
    this->secondary_db->load_project(fi.absoluteDir().path());
    this->list_maps();
}


void ImportDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    this->id = current->text(2).toInt();
    this->display_map();
}

