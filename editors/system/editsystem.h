#ifndef EDITSYSTEM_H
#define EDITSYSTEM_H

#include <QWidget>

namespace Ui {
class EditSystem;
}

class RPGEditorController;

class EditSystem : public QWidget
{
    Q_OBJECT

public:
    explicit EditSystem(QWidget *parent = nullptr);
    ~EditSystem();

    void setEC(RPGEditorController *ec);

private:
    Ui::EditSystem *ui;
    RPGEditorController *ec;
};

#endif // EDITSYSTEM_H
