#ifndef EDITSTATES_H
#define EDITSTATES_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class EditStates;
}

class RPGEditorController;

class EditStates : public QWidget
{
    Q_OBJECT

public:
    explicit EditStates(QWidget *parent = nullptr);
    ~EditStates();


    void setEC(RPGEditorController *ec);

    void set_state(int n);
    void save();

private:
    Ui::EditStates *ui;
    RPGEditorController *ec;
    int current;
};

#endif // EDITSTATES_H
