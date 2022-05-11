#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QToolButton>
#include <QFileDialog>
#include <QSettings>
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

private slots:
    void on_actionLayer1_triggered();

    void on_actionLayer2_triggered();

    void on_actionLayer3_triggered();

    void on_actionEvents_triggered();

    void on_actionDim_other_Layers_toggled(bool arg1);

    void on_actionOpen_triggered();


    void on_actionPen_triggered();

    void on_actionSelect_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionDelete_triggered();

    void on_actionFlood_Fill_triggered();


private:
    Ui::MainWindow *ui;
    TilesetWidget *tileset_widget;
    QString current_project_dir;
    QActionGroup *layergroup;
    QActionGroup *modegroup;

    QList<int> clipboard;
};
#endif // MAINWINDOW_H
