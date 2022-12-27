#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "RXIO/testcases.h"
#include "RXIO/ioactorfile.h"
#include "RXIO/ioclassfile.h"
#include "RXIO/ioskillfile.h"
#include "RXIO/ioitemfile.h"
#include "RXIO/ioweaponfile.h"
#include "RXIO/ioarmorfile.h"
#include "RXIO/ioenemyfile.h"
#include "RXIO/iotroopfile.h"
#include "RXIO/iostatefile.h"
#include "RXIO/ioanimationfile.h"

#include "RXIO/iocommoneventfile.h"
#include "RXIO/RXObjects/rpgmap.h"
#include "RXIO/RXObjects/rpgmapinfo.h"

#include "RXIO/ioscriptfile.h"




#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"


#include <QDebug>
#include <QToolButton>
#include <QActionGroup>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->map_tree_widget->setSystem(&this->system);

    this->ui->mapView->setSystem(&this->system);
    connect(this->ui->mapView, SIGNAL(zoom_in()), this, SLOT(zoom_in()));
    connect(this->ui->mapView, SIGNAL(zoom_out()), this, SLOT(zoom_out()));
    connect(this->ui->mapView, SIGNAL(one_tile_selected(int)), this->ui->tilesetView, SLOT(select_tile(int)));
    connect(this->ui->mapView, SIGNAL(mouse_over_coordinates(int,int)), this, SLOT(show_current_coordinates(int,int)));

    this->ui->tilesetView->setDB(&this->db);
    connect(this->ui->tilesetView, SIGNAL(selection_changed(QList<int>)), this->ui->mapView, SLOT(set_brush(QList<int>)));

    this->ui->splitter->setSizes(QList<int>({3, 3}));

    connect(this->ui->map_tree_widget, SIGNAL(on_map_selected(int)), this, SLOT(change_map(int)));


    this->layergroup = new QActionGroup(this);
    this->layergroup->addAction(this->ui->actionLayer1);
    this->layergroup->addAction(this->ui->actionLayer2);
    this->layergroup->addAction(this->ui->actionLayer3);

    this->modegroup = new QActionGroup(this);
    this->modegroup->addAction(this->ui->actionPen);
    this->modegroup->addAction(this->ui->actionSelect);
    this->modegroup->addAction(this->ui->actionFlood_Fill);
    this->modegroup->addAction(this->ui->actionEvents);

    this->viewgroup = new QActionGroup(this);
    this->modegroup->addAction(this->ui->actionCurrent_Layers_and_below);
    this->modegroup->addAction(this->ui->actionAll_Layers);



    this->ui->actionLayer1->setChecked(true);
    this->ui->actionPen->setChecked(true);
    this->ui->actionAll_Layers->setChecked(true);

    //Must be here as columns are set in QtCreator->MainWindow file.
    this->ui->map_tree_widget->hideColumn(1);
    this->ui->map_tree_widget->hideColumn(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_project(QString project_path)
{
    QFileInfo fi(project_path);

    if (project_path != "" && fi.exists() && fi.isFile())
    {
        //RXIO2
        this->db.load_project(fi.absoluteDir().path());

        //RXIO1
        this->system.current_project_dir = fi.absoluteDir().path();
        this->system.data_dir = this->system.current_project_dir + QDir::separator() + "Data" + QDir::separator();
        this->system.graphics_dir = this->system.current_project_dir + QDir::separator() + "Graphics" + QDir::separator();
        this->system.tileset_dir = this->system.current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator();
        this->system.autotiles_dir = this->system.current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator();
        this->system.characters_dir = this->system.current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Characters" + QDir::separator();
        this->system.battlers_dir = this->system.current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Battlers" + QDir::separator();
        this->system.audio_dir = this->system.current_project_dir + QDir::separator() + "Audio" + QDir::separator();

        try{
            IOSystemFile systemfile(this->system.data_dir + "System.rxdata", &this->system);
            system.tileset_list.clear();
            system.tileset_hash.clear();
            IOTilesetFile tileset_file(this->system.data_dir + "Tilesets.rxdata", &this->system.tileset_hash, &this->system.tileset_list);
            system.map_info_list.clear();
            IOMapInfoFile mapinfo_file(this->system.data_dir + "MapInfos.rxdata", &system.map_info_list);
            for (int i = 0; i < this->system.tileset_list.length(); i++)
                this->system.tileset_list.at(i)->load_tileset_graphic(&this->system);

            //maybe do all following files in a new try catch ... as they are not neccessary for map editing
            system.actor_list.clear();
            IOActorFile actor_file(this->system.data_dir + "Actors.rxdata", &this->system.actor_list);
            system.classes_list.clear();
            IOClassFile class_file(this->system.data_dir + "Classes.rxdata", &this->system.classes_list);
            system.skills_list.clear();
            IOSKillFile skill_file(this->system.data_dir + "Skills.rxdata", &this->system.skills_list);
            system.items_list.clear();
            IOItemFile item_file(this->system.data_dir + "Items.rxdata", &this->system.items_list);
            system.weapons_list.clear();
            IOWeaponFile weapon_file(this->system.data_dir + "Weapons.rxdata", &this->system.weapons_list);
            system.armors_list.clear();
            IOArmorFile armor_file(this->system.data_dir + "Armors.rxdata", &this->system.armors_list);
            system.enemies_list.clear();
            IOEnemyFile enemies_file(this->system.data_dir + "Enemies.rxdata", &this->system.enemies_list);
            system.troops_list.clear();
            IOTroopFile troops_file(this->system.data_dir + "Troops.rxdata", &this->system.troops_list);
            system.states_list.clear();
            IOStateFile states_file(this->system.data_dir + "States.rxdata", &this->system.states_list);

            IOAnimationFile animation_file(this->system.data_dir + "Animations.rxdata", &this->system.animation_list);
            IOCommonEventFile commonevents_file(this->system.data_dir + "CommonEvents.rxdata", &this->system.common_events_list);

        }
        catch(ParserException *ex)
        {
            qDebug() << "exception when reading files";
            this->ui->map_tree_widget->handleParserException(ex);
        }
    }

    this->ui->map_tree_widget->list_maps();
}

void MainWindow::change_map(int id)
{
    this->ui->mapView->set_map(id);
    this->system.current_map_id = id;
    this->ui->tilesetView->set_tileset(this->system.map_info_list.at(id)->map->tileset_id);


    //TODO: restore this
    /*
    if (this->ui->map_label->has_map_changed())
    {
        QMessageBox msgbox;
        msgbox.setText("There are unsaved changes in this map! Save to file?");
        QPushButton *save = msgbox.addButton("Save to File", QMessageBox::YesRole);
        QPushButton *withdraw = msgbox.addButton("Withdraw Changes", QMessageBox::NoRole);
        QPushButton *cancel = msgbox.addButton("Cancel", QMessageBox::AcceptRole);

        msgbox.exec(); //ignore return

        if (msgbox.clickedButton() == (QAbstractButton*)save)
        {
            //this->ui->map_label->do_save();
        }
        else if (msgbox.clickedButton() == (QAbstractButton*)withdraw)
        {
            //withdraw chances
            //this->ui->map_label->do_withdraw();
        }
        else if (msgbox.clickedButton() == (QAbstractButton*)cancel)
        {
            //cancel
            return; // dont change anything
        }
    }
    //this->ui->map_label->set_map(id);
    this->system.current_map_id = id;
    this->ui->autotiles_label->change_tileset(this->system.map_info_list.at(id)->map->tileset_id);
    this->ui->tileset_label->change_tileset(this->system.map_info_list.at(id)->map->tileset_id);
    this->ui->tileset_label_2->change_tileset(this->system.map_info_list.at(id)->map->tileset_id);
    */
}

void MainWindow::on_actionLayer1_triggered()
{
    this->ui->mapView->set_layer(0);
}

void MainWindow::on_actionLayer2_triggered()
{
    this->ui->mapView->set_layer(1);
}

void MainWindow::on_actionLayer3_triggered()
{
    this->ui->mapView->set_layer(2);
}

void MainWindow::on_actionEvents_triggered()
{
    this->ui->actionPen->setChecked(false);
    this->ui->actionSelect->setChecked(false);
    this->ui->mapView->set_mode(MapView::EVENT);
}

void MainWindow::on_actionOpen_triggered()
{
    QSettings settings;
    QString dir = settings.value("lastopened").toString();
    if (dir.isEmpty())
        dir = QDir::homePath();

    QString name = QFileDialog::getOpenFileName(this, "Choose Project", dir, "RPG Maker Project Files (*.rxproj);;Data Files(*.rxdata)");
    if (name != "")
        settings.setValue("lastopened", name);

    this->open_project(name);
}

void MainWindow::on_actionPen_triggered()
{
    this->ui->mapView->set_mode(MapView::PEN);
    this->ui->actionPen->setChecked(true); // if activated per signal, pen should be selected in gui
}

void MainWindow::on_actionSelect_triggered()
{
    this->ui->mapView->set_mode(MapView::SELECT);
}

void MainWindow::on_actionCut_triggered()
{
    this->clipboard = this->ui->mapView->do_cut();
}

void MainWindow::on_actionCopy_triggered()
{
    this->clipboard = this->ui->mapView->do_copy();
}

void MainWindow::on_actionPaste_triggered()
{
    if (this->clipboard.length() >= 3)
        this->ui->mapView->do_paste(this->clipboard);
}

void MainWindow::on_actionDelete_triggered()
{
    this->ui->mapView->do_delete();
}

void MainWindow::on_actionFlood_Fill_triggered()
{
    //this->ui->map_label->set_mode(MapWidget::FLOOD);
}



void MainWindow::on_actionAll_Layers_triggered()
{
    this->ui->mapView->set_current_and_below(false);
}

void MainWindow::on_actionCurrent_Layers_and_below_triggered()
{
    this->ui->mapView->set_current_and_below(true);
}

void MainWindow::on_actionImport_RGSSAD_triggered()
{
    QSettings settings;
    QString dir = settings.value("lastopened").toString();
    if (dir.isEmpty())
        dir = QDir::homePath();

    QString name = QFileDialog::getOpenFileName(this, "Choose Project", dir, "RGSSAD File (*.rgssad);;");
    settings.setValue("lastopened", name);
    QFileInfo fi(name);

    if (name != "" && fi.exists() && fi.isFile())
    {
        QString export_dir = fi.dir().absolutePath() + QDir::separator();
        QDir().mkdir(fi.dir().absolutePath() + QDir::separator() + "Data" + QDir::separator());
        try{
            IORGSSAD rgssad_file(name,export_dir);
        }
        catch(ParserException *ex)
        {
            this->ui->map_tree_widget->handleParserException(ex);
        }
    }

    this->open_project(name);
}

void MainWindow::on_actionRun_Testcases_triggered()
{
    Testcases testcases(&this->system);
    if (testcases.ok)
        QMessageBox::information(this, "Run Testcases","Testcases successfull");
    else
        QMessageBox::critical(this, "Run Testcases","Testcases failed! Please contact developer!");
}

void MainWindow::show_current_coordinates(int x, int y)
{
    this->ui->statusbar->showMessage(QString("x: %1, y: %2").arg(x,3,10,QChar('0')).arg(y,3,10,QChar('0')));
}

void MainWindow::on_actionActors_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::ACTORS);
    da->show();
}

void MainWindow::on_actionClasses_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::CLASSES);
    da->show();
}

void MainWindow::on_actionSkills_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::SKILLS);
    da->show();
}

void MainWindow::on_actionItem_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::ITEMS);
    da->show();
}

void MainWindow::on_actionWeapons_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::WEAPONS);
    da->show();
}

void MainWindow::on_actionArmors_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::ARMORS);
    da->show();
}

void MainWindow::on_actionEnemies_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::ENEMIES);
    da->show();
}

void MainWindow::on_actionTroops_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::TROOPS);
    da->show();
}

void MainWindow::on_actionStates_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::STATES);
    da->show();
}

void MainWindow::on_actionAnimations_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::ANIMATIONS);
    da->show();
}

void MainWindow::on_actionTileset_Editor_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::TILESETS);
    da->show();
}

void MainWindow::on_actionCommon_Scripts_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::COMMONEVENTS);
    da->show();
}

void MainWindow::on_actionSystem_triggered()
{
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::SYSTEM);
    da->show();
}

void MainWindow::on_actionScripting_Editor_triggered()
{

}

void MainWindow::on_actionSave_triggered()
{
    //this->ui->map_label->do_save();
    //this->ui->map_label->set_map_changed(false);

    this->db.save_project();

    //this->ui->map_tree_widget->do_save();
}

void MainWindow::on_actionshift_up_triggered()
{
    this->ui->mapView->shift(MapView::UP, 1);
}

void MainWindow::on_actionShift_Down_triggered()
{
    this->ui->mapView->shift(MapView::DOWN, 1);
}

void MainWindow::on_actionShift_right_triggered()
{
    this->ui->mapView->shift(MapView::RIGHT, 1);
}

void MainWindow::on_actionShift_left_triggered()
{
    this->ui->mapView->shift(MapView::LEFT, 1);
}

void MainWindow::on_actionExtend_Left_triggered()
{
    this->ui->mapView->extend(MapView::LEFT, 1);
}

void MainWindow::on_actionExtend_Right_triggered()
{
    this->ui->mapView->extend(MapView::RIGHT, 1);
}

void MainWindow::on_actionExtend_Up_triggered()
{
    this->ui->mapView->extend(MapView::UP, 1);
}

void MainWindow::on_actionExtend_Down_triggered()
{
    this->ui->mapView->extend(MapView::DOWN, 1);
}

void MainWindow::on_actionCrop_Left_triggered()
{
    bool ok;
    int val = QInputDialog::getInt(this, "Crop Left", "How many tiles should be cropped?", 1, 1, 20, 1, &ok);
    if (ok)
        this->ui->mapView->crop(MapView::LEFT, val);
}

void MainWindow::on_actionCrop_Right_triggered()
{
    bool ok;
    int val = QInputDialog::getInt(this, "Crop Right", "How many tiles should be cropped?", 1, 1, 20, 1, &ok);
    if (ok)
        this->ui->mapView->crop(MapView::RIGHT, val);
}

void MainWindow::on_actionCrop_Up_triggered()
{
    bool ok;
    int val = QInputDialog::getInt(this, "Crop Up", "How many tiles should be cropped?", 1, 1, 20, 1, &ok);
    if (ok)
        this->ui->mapView->crop(MapView::UP, val);
}

void MainWindow::on_actionCrop_Down_triggered()
{
    bool ok;
    int val = QInputDialog::getInt(this, "Crop Down", "How many tiles should be cropped?", 1, 1, 20, 1, &ok);
    if (ok)
        this->ui->mapView->crop(MapView::DOWN, val);
}

void MainWindow::on_actionOpen_Project_Folder_triggered()
{
    QDesktopServices::openUrl( QUrl::fromLocalFile( system.current_project_dir ) );
}

void MainWindow::on_actionExport_Map_Images_triggered()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Export Map Images", QDir::homePath());
    if (dir != "")
    {
        for (int i = 0; i < system.map_info_list.length(); i++)
        {
            system.map_info_list.at(i)->load_map(&this->system);
            QImage img = system.map_info_list.at(i)->map->create_map_image(RPGMap::ZOOM_100,false,true,2,
                                                              system.tileset_list.at(system.map_info_list.at(i)->map->tileset_id-1));
            img.save(dir + QDir::separator() + system.map_info_list.at(i)->name + ".png");
        }
    }
}


void MainWindow::on_slider_scale_valueChanged(int value)
{
    qreal scale = qPow(qreal(2), (value - 250) / qreal(50));

    QTransform matrix;
    matrix.scale(scale,scale);
    this->ui->mapView->setTransform(matrix);
}

void MainWindow::zoom_in()
{
    this->ui->slider_scale->setValue(this->ui->slider_scale->value()+10);
}

void MainWindow::zoom_out()
{
    this->ui->slider_scale->setValue(this->ui->slider_scale->value()-10);
}
