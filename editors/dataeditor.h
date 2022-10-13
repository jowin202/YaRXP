#ifndef DATAEDITOR_H
#define DATAEDITOR_H

#include <QWidget>
#include <QDebug>

#include "../RXIO/RXObjects/rpgsystem.h"

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
    void setSystem(RPGSystem *system);


private slots:
    void on_object_list_currentRowChanged(int currentRow);
    void on_central_menu_currentRowChanged(int currentRow);


    void on_button_cancel_clicked();


    void on_button_apply_clicked();

private:
    Ui::DataEditor *ui;
    RPGSystem *system;

};

#endif // DATAEDITOR_H
