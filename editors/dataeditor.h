#ifndef DATAEDITOR_H
#define DATAEDITOR_H

#include <QWidget>
#include <QDebug>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QAction>
#include <QSettings>
#include <QMessageBox>

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

    enum {ACTORS, CLASSES, SKILLS, ITEMS, WEAPONS, ARMORS, ENEMIES, TROOPS, STATES, ANIMATIONS, TILESETS, COMMONEVENTS, SYSTEM, SCRIPTS};
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


    bool set_maximum(int obj_type);

    void on_scripts_list_doubleClicked(const QModelIndex &index);

    void on_button_remove_script_clicked();

    void on_buttton_add_script_clicked();

private:
    Ui::DataEditor *ui;
    RPGDB *db;
    RPGEditorController *ec;


    QAction action_actors_copy;
    QAction action_actors_paste;

    QAction action_classes_copy;
    QAction action_classes_paste;

    QAction action_skills_copy;
    QAction action_skills_paste;

    QAction action_items_copy;
    QAction action_items_paste;

    QAction action_weapons_copy;
    QAction action_weapons_paste;

    QAction action_armors_copy;
    QAction action_armors_paste;

    QAction action_enemies_copy;
    QAction action_enemies_paste;

    QAction action_troops_copy;
    QAction action_troops_paste;

    QAction action_states_copy;
    QAction action_states_paste;

    QAction action_animations_copy;
    QAction action_animations_paste;

    QAction action_tilesets_copy;
    QAction action_tilesets_paste;

    QAction action_common_events_copy;
    QAction action_common_events_paste;

    QAction action_scripts_copy;
    QAction action_scripts_paste;
};

#endif // DATAEDITOR_H
