#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "RXIO2/rpgdb.h"
#include "RXIO2/rpgeditorcontroller.h"
#include "RXIO2/iorgssad.h"


#include <QDebug>
#include <QToolButton>
#include <QActionGroup>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->map_tree_widget->setDB(&this->db);
    this->ui->mapView->setDB(&this->db);

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
    }

    this->ui->map_tree_widget->list_maps();
}

void MainWindow::change_map(int id)
{
    this->ui->mapView->set_map(id);
    this->ui->tilesetView->set_tileset(this->db.get_mapfile_by_id(id)->object().value("@tileset_id").toInt());
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
    //this->clipboard = this->ui->mapView->do_cut();
}

void MainWindow::on_actionCopy_triggered()
{
    if (this->ui->map_tree_widget->hasFocus())
    {
        this->ui->map_tree_widget->copy_map();
    }
    else
    {

    }
}

void MainWindow::on_actionPaste_triggered()
{
    if (this->ui->map_tree_widget->hasFocus())
    {
        this->ui->map_tree_widget->paste_map();
    }
    else
    {

    }
}

void MainWindow::on_actionDelete_triggered()
{
    if (this->ui->map_tree_widget->hasFocus())
    {
        this->ui->map_tree_widget->delete_map();
    }
    else
    {
        //this->clipboard = this->ui->mapView->do_copy();
    }
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
        IORGSSAD rgssad_file(name,export_dir);
    }

    this->open_project(name);
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
    DataEditor *da = new DataEditor;
    da->setDB(&this->db);
    da->set_widget(DataEditor::SCRIPTS);
    da->show();
}

void MainWindow::on_actionSave_triggered()
{
    this->db.save_project();
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
    QDesktopServices::openUrl( QUrl::fromLocalFile( db.project_dir ) );
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
