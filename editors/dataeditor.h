#ifndef DATAEDITOR_H
#define DATAEDITOR_H

#include <QWidget>
#include <QDebug>
#include <QInputDialog>

class RPGDB;
class RPGEditorController;


namespace Ui {
class DataEditor;
}

class DataEditor : public QWidget
{
    Q_OBJECT

public:
    explicit DataEditor(QWidget *parent = nullptr);
    ~DataEditor();

    enum {ACTORS, CLASSES, SKILLS, ITEMS, WEAPONS, ARMORS, ENEMIES, TROOPS, STATES, ANIMATIONS, TILESETS, COMMONEVENTS, SYSTEM};
    void set_widget(int widget);
    void setDB(RPGDB *db);


private slots:
    void on_button_cancel_clicked();
    void on_button_apply_clicked();
    void on_button_ok_clicked();


    void on_tabWidget_currentChanged(int index);

    void on_button_actor_max_clicked();
    void on_button_classes_max_clicked();
    void on_button_skills_max_clicked();
    void on_button_items_max_clicked();
    void on_button_weapons_max_clicked();
    void on_button_armors_max_clicked();
    void on_button_enemies_max_clicked();
    void on_button_troops_max_clicked();
    void on_button_states_max_clicked();
    void on_button_animations_max_clicked();
    void on_button_tilesets_max_clicked();
    void on_button_common_events_max_clicked();


    void set_maximum(int obj_type);

private:
    Ui::DataEditor *ui;
    RPGDB *db;
    RPGEditorController *ec;

};

#endif // DATAEDITOR_H
