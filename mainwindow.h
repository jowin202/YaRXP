#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QToolButton>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include <QMessageBox>

#include <QDesktopServices>

#include "editors/dataeditor.h"


#include "RXIO2/rpgdb.h"



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
    void change_map(int id);
    void on_actionLayer1_triggered();
    void on_actionLayer2_triggered();
    void on_actionLayer3_triggered();
    void on_actionEvents_triggered();
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
    void show_current_coordinates(int x,int y);

    void on_actionImport_RGSSAD_triggered();
    void on_actionActors_triggered();
    void on_actionClasses_triggered();
    void on_actionSkills_triggered();
    void on_actionItem_triggered();
    void on_actionWeapons_triggered();
    void on_actionArmors_triggered();
    void on_actionEnemies_triggered();
    void on_actionTroops_triggered();
    void on_actionStates_triggered();
    void on_actionAnimations_triggered();
    void on_actionTileset_Editor_triggered();
    void on_actionCommon_Scripts_triggered();
    void on_actionSystem_triggered();
    void on_actionScripting_Editor_triggered();
    void on_actionSave_triggered();
    void on_actionshift_up_triggered();
    void on_actionShift_Down_triggered();
    void on_actionShift_right_triggered();
    void on_actionShift_left_triggered();
    void on_actionExtend_Left_triggered();
    void on_actionExtend_Right_triggered();
    void on_actionExtend_Up_triggered();
    void on_actionExtend_Down_triggered();
    void on_actionCrop_Left_triggered();
    void on_actionCrop_Right_triggered();
    void on_actionCrop_Up_triggered();
    void on_actionCrop_Down_triggered();
    void on_actionOpen_Project_Folder_triggered();


    void on_slider_scale_valueChanged(int value);
    void zoom_in();
    void zoom_out();


    void on_actionUndo_triggered();

    void on_actionPBS_Editor_triggered();

    void on_actionSave_Map_Pic_triggered();

    void on_actionExport_Maps_to_Godot_triggered();

private:
    Ui::MainWindow *ui;
    QActionGroup *layergroup;
    QActionGroup *modegroup;
    QActionGroup *viewgroup;

    RPGDB db;


};
#endif // MAINWINDOW_H
