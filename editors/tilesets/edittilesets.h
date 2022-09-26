#ifndef EDITTILESETS_H
#define EDITTILESETS_H

#include <QWidget>

namespace Ui {
class EditTilesets;
}

class RPGSystem;
class EditTilesets : public QWidget
{
    Q_OBJECT

public:
    explicit EditTilesets(QWidget *parent = nullptr);
    ~EditTilesets();

    void setSystem(RPGSystem *system)
    {this->system = system;}

    void set_tileset(int n);


private:
    Ui::EditTilesets *ui;
    RPGSystem *system;
};

#endif // EDITTILESETS_H
