#ifndef DATAEDITOR_H
#define DATAEDITOR_H

#include <QWidget>
#include <QDebug>

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

private slots:
    void on_central_menu_itemSelectionChanged();

private:
    Ui::DataEditor *ui;
};

#endif // DATAEDITOR_H
