#ifndef EDITCOMMONEVENTS_H
#define EDITCOMMONEVENTS_H

#include <QWidget>

namespace Ui {
class EditCommonEvents;
}

class RPGSystem;
class EditCommonEvents : public QWidget
{
    Q_OBJECT

public:
    explicit EditCommonEvents(QWidget *parent = nullptr);
    ~EditCommonEvents();

    void setSystem(RPGSystem *system);


    void set_commonevent(int n);

private slots:

    void on_combo_trigger_currentIndexChanged(int index);

private:
    Ui::EditCommonEvents *ui;
    RPGSystem *system;
};

#endif // EDITCOMMONEVENTS_H
