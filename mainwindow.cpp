#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tilesetwidget.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this->ui->tileset_label, SIGNAL(selection_changed(QList<int>)), this->ui->map_label, SLOT(set_selection(QList<int>)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test_slot()
{
    qDebug() << "bla blubb";
}

