#ifndef EDITITEMS_H
#define EDITITEMS_H

#include <QWidget>

namespace Ui {
class EditItems;
}

class RPGSystem;

class EditItems : public QWidget
{
    Q_OBJECT

public:
    explicit EditItems(QWidget *parent = nullptr);
    ~EditItems();

    void setSystem(RPGSystem *system)
    {this->system = system;}

    void set_item(int n);


private slots:
    void on_combo_parameter_currentIndexChanged(int index);

private:
    Ui::EditItems *ui;
    RPGSystem *system;
};

#endif // EDITITEMS_H
