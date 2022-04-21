#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tilesetwidget.h"

#include <QDebug>
#include <QToolButton>
#include <QActionGroup>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->tileset_label, SIGNAL(selection_changed(QList<int>)), this->ui->map_label, SLOT(set_brush(QList<int>)));

    this->layergroup = new QActionGroup(this);
    this->layergroup->addAction(this->ui->actionLayer1);
    this->layergroup->addAction(this->ui->actionLayer2);
    this->layergroup->addAction(this->ui->actionLayer3);
    this->layergroup->addAction(this->ui->actionEvents);

    this->modegroup = new QActionGroup(this);
    this->modegroup->addAction(this->ui->actionPen);
    this->modegroup->addAction(this->ui->actionSelect);


    this->ui->actionLayer1->setChecked(true);
    this->ui->actionPen->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test_slot()
{
    qDebug() << "bla blubb";
}


void MainWindow::on_actionLayer1_triggered()
{
    this->ui->map_label->set_layer(0);
    //this->ui->actionLayer2->setChecked(false);
    //this->ui->actionLayer3->setChecked(false);
}

void MainWindow::on_actionLayer2_triggered()
{
    this->ui->map_label->set_layer(1);
    //this->ui->actionLayer1->setChecked(false);
    //this->ui->actionLayer3->setChecked(false);
}

void MainWindow::on_actionLayer3_triggered()
{
    this->ui->map_label->set_layer(2);
    //this->ui->actionLayer1->setChecked(false);
    //this->ui->actionLayer2->setChecked(false);
}

void MainWindow::on_actionEvents_triggered()
{

}

void MainWindow::on_actionDim_other_Layers_toggled(bool arg1)
{
    this->ui->map_label->set_dim(arg1);
}

void MainWindow::on_actionOpen_triggered()
{
    QString name = QFileDialog::getOpenFileName(this, "Choose Project", QDir::homePath(), "RPG Maker Project Files (*.rxproj);;Data Files(*.rxdata)");
    //QString name = "/home/johannes/RPG_maker/Pokemon Klagenfurt Vibes/Game.rxproj";
    QFileInfo fi(name);

    if (name != "" && fi.exists() && fi.isFile())
    {
        this->current_project_dir = fi.absoluteDir().path();
        this->ui->map_tree_widget->list_maps(this->current_project_dir);
    }
}

void MainWindow::on_actionPen_triggered()
{
    this->ui->map_label->set_mode(MapWidget::PEN);
}

void MainWindow::on_actionSelect_triggered()
{
    this->ui->map_label->set_mode(MapWidget::SELECT);
}
