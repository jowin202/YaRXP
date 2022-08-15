#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tilesetwidget.h"
#include "RXIO/testcases.h"

#include <QDebug>
#include <QToolButton>
#include <QActionGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->map_tree_widget->setSettings(&this->settings);
    this->ui->map_label->setSettings(&this->settings);

    //tilesets need 3 widgets
    this->ui->autotiles_label->setSettings(&this->settings);
    this->ui->tileset_label->setSettings(&this->settings);
    this->ui->tileset_label_2->setSettings(&this->settings);
    this->ui->autotiles_label->setRange(0); //autotiles
    this->ui->tileset_label->setRange(1); //normal tiles
    this->ui->tileset_label_2->setRange(2); //extension if tileset longer than 32768
    connect(this->ui->autotiles_label, SIGNAL(selection_changed(QList<int>)), this->ui->map_label, SLOT(set_brush(QList<int>)));
    connect(this->ui->autotiles_label, SIGNAL(selection_changed(QList<int>)), this, SLOT(on_actionPen_triggered())); //set pen when select tile
    connect(this->ui->tileset_label, SIGNAL(selection_changed(QList<int>)), this->ui->map_label, SLOT(set_brush(QList<int>)));
    connect(this->ui->tileset_label, SIGNAL(selection_changed(QList<int>)), this, SLOT(on_actionPen_triggered())); //set pen when select tile
    connect(this->ui->tileset_label_2, SIGNAL(selection_changed(QList<int>)), this->ui->map_label, SLOT(set_brush(QList<int>)));
    connect(this->ui->tileset_label_2, SIGNAL(selection_changed(QList<int>)), this, SLOT(on_actionPen_triggered())); //set pen when select tile

    //widget lose focus if clicked on other widget
    connect(this->ui->autotiles_label, SIGNAL(selection_changed(QList<int>)), this->ui->tileset_label, SLOT(updateView()));
    connect(this->ui->autotiles_label, SIGNAL(selection_changed(QList<int>)), this->ui->tileset_label_2, SLOT(updateView()));
    connect(this->ui->tileset_label, SIGNAL(selection_changed(QList<int>)), this->ui->autotiles_label, SLOT(updateView()));
    connect(this->ui->tileset_label, SIGNAL(selection_changed(QList<int>)), this->ui->tileset_label_2, SLOT(updateView()));
    connect(this->ui->tileset_label_2, SIGNAL(selection_changed(QList<int>)), this->ui->autotiles_label, SLOT(updateView()));
    connect(this->ui->tileset_label_2, SIGNAL(selection_changed(QList<int>)), this->ui->tileset_label, SLOT(updateView()));


    connect(this->ui->map_tree_widget, SIGNAL(on_map_selected(RPGMap*)), this->ui->map_label, SLOT(set_map(RPGMap*)));
    connect(this->ui->map_tree_widget, SIGNAL(on_tileset_changed(int)), this->ui->autotiles_label, SLOT(change_tileset(int)));
    connect(this->ui->map_tree_widget, SIGNAL(on_tileset_changed(int)), this->ui->tileset_label, SLOT(change_tileset(int)));
    connect(this->ui->map_tree_widget, SIGNAL(on_tileset_changed(int)), this->ui->tileset_label_2, SLOT(change_tileset(int)));


    this->layergroup = new QActionGroup(this);
    this->layergroup->addAction(this->ui->actionLayer1);
    this->layergroup->addAction(this->ui->actionLayer2);
    this->layergroup->addAction(this->ui->actionLayer3);
    this->layergroup->addAction(this->ui->actionEvents);

    this->modegroup = new QActionGroup(this);
    this->modegroup->addAction(this->ui->actionPen);
    this->modegroup->addAction(this->ui->actionSelect);
    this->modegroup->addAction(this->ui->actionFlood_Fill);

    this->viewgroup = new QActionGroup(this);
    this->modegroup->addAction(this->ui->actionCurrent_Layers_and_below);
    this->modegroup->addAction(this->ui->actionAll_Layers);


    this->ui->actionLayer1->setChecked(true);
    this->ui->actionPen->setChecked(true);
    this->ui->actionAll_Layers->setChecked(true);

    this->ui->map_tree_widget->hideColumn(1);
    this->ui->map_tree_widget->hideColumn(2);
    this->ui->map_tree_widget->hideColumn(3);
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
        this->settings.current_project_dir = fi.absoluteDir().path();
        this->settings.data_dir = this->settings.current_project_dir + QDir::separator() + "Data" + QDir::separator();
        this->settings.grahpics_dir = this->settings.current_project_dir + QDir::separator() + "Graphics" + QDir::separator();
        this->settings.tileset_dir = this->settings.current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Tilesets" + QDir::separator();
        this->settings.autotiles_dir = this->settings.current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Autotiles" + QDir::separator();
        this->settings.characters_dir = this->settings.current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Characters" + QDir::separator();
        this->settings.characters_dir = this->settings.current_project_dir + QDir::separator() + "Graphics" + QDir::separator() + "Characters" + QDir::separator();
        this->ui->map_tree_widget->list_maps();


        try{
            IOSystemFile systemfile(this->settings.data_dir + "System.rxdata", &this->settings);
        }
        catch(ParserException *ex)
        {
            this->ui->map_tree_widget->handleParserException(ex);
        }
    }
}

void MainWindow::on_actionLayer1_triggered()
{
    this->ui->map_label->set_layer(0);
}

void MainWindow::on_actionLayer2_triggered()
{
    this->ui->map_label->set_layer(1);
}

void MainWindow::on_actionLayer3_triggered()
{
    this->ui->map_label->set_layer(2);
}

void MainWindow::on_actionEvents_triggered()
{
    this->ui->map_label->set_event_mode();
}

void MainWindow::on_actionDim_other_Layers_toggled(bool arg1)
{
    this->ui->map_label->set_dim(arg1);
}

void MainWindow::on_actionOpen_triggered()
{
    QSettings settings;
    QString dir = settings.value("lastopened").toString();
    if (dir.isEmpty())
        dir = QDir::homePath();

    QString name = QFileDialog::getOpenFileName(this, "Choose Project", dir, "RPG Maker Project Files (*.rxproj);;Data Files(*.rxdata)");
    settings.setValue("lastopened", name);

    this->open_project(name);
}

void MainWindow::on_actionPen_triggered()
{
    this->ui->map_label->set_mode(MapWidget::PEN);
    this->ui->actionPen->setChecked(true); // if activated per signal, pen should be selected in gui
}

void MainWindow::on_actionSelect_triggered()
{
    this->ui->map_label->set_mode(MapWidget::SELECT);
}

void MainWindow::on_actionCut_triggered()
{
    this->clipboard = this->ui->map_label->do_cut();
}

void MainWindow::on_actionCopy_triggered()
{
    this->clipboard = this->ui->map_label->do_copy();
}

void MainWindow::on_actionPaste_triggered()
{
    if (this->clipboard.length() >= 3)
        this->ui->map_label->do_paste(this->clipboard);
}

void MainWindow::on_actionDelete_triggered()
{
    this->ui->map_label->do_delete();
}

void MainWindow::on_actionFlood_Fill_triggered()
{
    this->ui->map_label->set_mode(MapWidget::FLOOD);
}





void MainWindow::on_actionAll_Layers_triggered()
{
    this->ui->map_label->set_show_other_layers(true);
}

void MainWindow::on_actionCurrent_Layers_and_below_triggered()
{
    this->ui->map_label->set_show_other_layers(false);
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
    Testcases testcases(&this->settings);
    if (testcases.ok)
        QMessageBox::information(this, "Run Testcases","Testcases successfull");
    else
        QMessageBox::critical(this, "Run Testcases","Testcases failed! Please contact developer!");
}
