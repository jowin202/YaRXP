#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tilesetwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void test_signal();

public slots:
    void test_slot();

private:
    Ui::MainWindow *ui;
    TilesetWidget *tileset_widget;
};
#endif // MAINWINDOW_H
