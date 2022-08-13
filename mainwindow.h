#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QToolButton>
#include <QFileDialog>
#include <QSettings>

#include "tilesetwidget.h"
#include "autotileset.h"
#include "RXIO/RXObjects/rpgsettings.h"

#include "RXIO/iosystemfile.h"
#include "RXIO/iorgssad.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void open_project(QString project_path);

signals:
    void test_signal();

public slots:
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
    void on_actionAll_Layers_triggered();
    void on_actionCurrent_Layers_and_below_triggered();

private slots:
    void on_actionImport_RGSSAD_triggered();

private:
    Ui::MainWindow *ui;
    QActionGroup *layergroup;
    QActionGroup *modegroup;
    QActionGroup *viewgroup;

    QList<int> clipboard; //only for copy paste at map (no events


    RPGSettings settings;

};
#endif // MAINWINDOW_H
