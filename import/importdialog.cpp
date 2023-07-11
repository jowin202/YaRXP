#include "importdialog.h"
#include "ui_importdialog.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/fileopener.h"
#include "RXIO2/rpgmapcontroller.h"
#include "RXIO2/rpgmapinfocontroller.h"

#include "RXIO2/autotileset.h"

#include "tilesetcompare.h"

ImportDialog::ImportDialog(RPGDB *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    this->db = db;
    this->db->fill_combo(this->ui->combo_tileset,RPGDB::TILESETS, true, 3);
    this->ui->combo_tileset->setCurrentIndex(0);
    connect(this->ui->combo_tileset, &QComboBox::currentIndexChanged, [=](){ this->display_maps(); });

    this->ui->treeWidget->hideColumn(1);
    this->ui->treeWidget->hideColumn(2);

    this->ui->splitter_2->setSizes(QList<int>() << 100 << 500);

    connect(this->ui->scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), this->ui->scrollArea_2->verticalScrollBar(), SLOT(setValue(int)));
    connect(this->ui->scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)), this->ui->scrollArea_2->horizontalScrollBar(), SLOT(setValue(int)));
    connect(this->ui->scrollArea_2->verticalScrollBar(), SIGNAL(valueChanged(int)), this->ui->scrollArea->verticalScrollBar(), SLOT(setValue(int)));
    connect(this->ui->scrollArea_2->horizontalScrollBar(), SIGNAL(valueChanged(int)), this->ui->scrollArea->horizontalScrollBar(), SLOT(setValue(int)));
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
        //item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        //item->setCheckState(0, Qt::Unchecked);
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
                //item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
                //item->setCheckState(0, Qt::Unchecked);
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
    this->ui->treeWidget->resizeColumnToContents(0);
}

void ImportDialog::display_maps()
{
    if (secondary_db == 0)
        return;

    if (secondary_db->get_mapfile_by_id(id) == 0)
        return;
    map = secondary_db->get_mapfile_by_id(id)->object();
    if (map.value("RXClass").toString() != "RPG::Map")
        return;


    this->draw_map_to_label(secondary_db,this->ui->label_before,
                            secondary_db->get_tileset_by_id(map.value("@tileset_id").toInt()),
                            secondary_db->get_mapfile_by_id(id)->object());

    this->draw_map_to_label(db,this->ui->label_after,
                            db->get_tileset_by_id(this->ui->combo_tileset->currentData().toInt()),
                            adjusted_map);
}

void ImportDialog::draw_map_to_label(RPGDB *currentdb, QLabel *target, QJsonObject tileset, QJsonObject map)
{
    QJsonObject map_data = map.value("@data").toObject();
    int width = map_data.value("x").toInt(10);
    int height = map_data.value("y").toInt(10);

    QImage tileset_img = FileOpener(currentdb->tileset_dir,tileset.value("@tileset_name").toString()).get_image();
    QImage autotiles_img = this->get_autotiles_image(currentdb,tileset);


    QImage img(width*32,height*32,QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&img);
    painter.fillRect(0,0,img.width(), img.height(), Qt::black);
    for (int z = 0; z < 3; z++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int pos = x + y * width + height*width * z;
                int id = map_data.value("values").toArray().at(pos).toInt();
                if (id < 0x180 && id >= 48)
                {
                    //autotiles
                    painter.drawImage(32*x,32*y,autotiles_img.copy(32*(id % 8), 32*(id / 8), 32, 32));
                }
                else if (id >= 0x180)
                {
                    id -= 0x180;
                    painter.drawImage(32*x,32*y,tileset_img.copy(32*(id % 8), 32*(id / 8), 32, 32));
                }
            }
        }
    }
    painter.end();
    target->setPixmap(QPixmap::fromImage(img));
}

QImage ImportDialog::get_autotiles_image(RPGDB *currentdb, QJsonObject tileset)
{
    QImage result(8*32,6*32*8, QImage::Format_ARGB32_Premultiplied);

    for (int y = 0; y < result.height(); y++)
        for (int x = 0; x < result.width(); x++)
            result.setPixelColor(x,y,QColor(0,0,0,0));

    QPainter painter(&result);

    QJsonArray autotile_names = tileset.value("@autotile_names").toArray();

    for (int i = 0; i < 7; i++)
    {
        painter.drawImage(0,6*32*(i+1),Autotileset(FileOpener(currentdb->autotiles_dir, autotile_names.at(i).toString()).get_image()).get_full_tileset());
    }
    painter.end();
    return result;
}

void ImportDialog::on_button_browse_clicked()
{
    QSettings settings;

    QString name = QFileDialog::getOpenFileName(this, "Choose Project", settings.value("import_path").toString(), "RPG Maker Project Files (*.rxproj);;Data Files(*.rxdata)");
    if (name == "")
        return;
    settings.setValue("import_path", name);

    this->ui->line_location->setText(name);

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
    this->adjusted_map = secondary_db->get_mapfile_by_id(id)->object();
    this->display_maps();
}


void ImportDialog::on_button_import_clicked()
{
    adjusted_map.insert("@tileset_id", this->ui->combo_tileset->currentData().toInt());
    RPGMapInfoController mic(db);
    int id = mic.get_lowest_available_id();
    mic.create_map_from_object(id, adjusted_map);
    mic.set_name(id, this->ui->treeWidget->currentItem()->text(0));
    emit finished();
}


void ImportDialog::on_button_adjust_clicked()
{
    QJsonObject map = secondary_db->get_mapfile_by_id(id)->object();
    if (map.value("RXClass").toString() != "RPG::Map")
        return;

    QJsonObject tileset_orig = secondary_db->get_tileset_by_id(map.value("@tileset_id").toInt());
    QJsonObject tileset_new = db->get_tileset_by_id(this->ui->combo_tileset->currentData().toInt());

    if (tileset_orig.value("RXClass").toString() != "RPG::Tileset" && tileset_new.value("RXClass").toString() != "RPG::Tileset")
        return;

    QImage tileset_img_orig = FileOpener(secondary_db->tileset_dir,tileset_orig.value("@tileset_name").toString()).get_image();
    QImage tileset_img_new = FileOpener(db->tileset_dir,tileset_new.value("@tileset_name").toString()).get_image();

    QImage autotiles_new = this->get_autotiles_image(db,tileset_new);
    QImage autotiles_orig = this->get_autotiles_image(secondary_db,tileset_orig);


    TilesetCompare *compare = new TilesetCompare(tileset_img_new, tileset_img_orig, autotiles_new, autotiles_orig, map);
    connect(compare, SIGNAL(progress_start(int)), this->ui->progressBar, SLOT(setMaximum(int)));
    connect(compare, SIGNAL(progress(int)), this->ui->progressBar, SLOT(setValue(int)));
    connect(compare, &TilesetCompare::has_result, [=](QJsonObject map)
    {
        this->adjusted_map = map;
        this->display_maps();
    });
    connect(compare, SIGNAL(finished()), compare, SLOT(deleteLater()));
    compare->start();
}


void ImportDialog::on_button_import_w_tileset_clicked()
{
    int res = QMessageBox::question(this, "Import Tileset Graphic Files", "Do you want to import the graphic files of the other project?");
    if (res == QMessageBox::Yes)
    {

        QJsonObject tileset = secondary_db->get_tileset_by_id(map.value("@tileset_id").toInt());
        int new_tileset_id = db->add_tileset(tileset);

        map.insert("@tileset_id", new_tileset_id);
        RPGMapInfoController mic(db);
        int id = mic.get_lowest_available_id();
        mic.create_map_from_object(id, map);
        mic.set_name(id, this->ui->treeWidget->currentItem()->text(0));


        QFile file(FileOpener(secondary_db->tileset_dir, tileset.value("@tileset_name").toString()).get_image_path());
        file.copy(db->tileset_dir + QFileInfo(file.fileName()).fileName());

        for (int i = 0; i < 7; i++)
        {
            QFile file(FileOpener(secondary_db->autotiles_dir, tileset.value("@autotile_names").toArray().at(i).toString()).get_image_path());
            file.copy(db->autotiles_dir + QFileInfo(file.fileName()).fileName());
        }

        emit finished();
    }


}

