#ifndef EDITCLASSES_H
#define EDITCLASSES_H

#include <QWidget>

class RPGSystem;

namespace Ui {
class EditClasses;
}

class EditClasses : public QWidget
{
    Q_OBJECT

public:
    explicit EditClasses(QWidget *parent = nullptr);
    ~EditClasses();

    void setSystem(RPGSystem *system) {this->system = system;}

public slots:
    void set_class(int n);

private:
    Ui::EditClasses *ui;
    RPGSystem *system = 0;
};

#endif // EDITCLASSES_H
