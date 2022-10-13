#ifndef CHECKBOXLIST_H
#define CHECKBOXLIST_H

#include <QWidget>

namespace Ui {
class CheckBoxList;
}

class RPGSystem;

class CheckBoxList : public QWidget
{
    Q_OBJECT

public:
    explicit CheckBoxList(QWidget *parent = nullptr);
    ~CheckBoxList();

    void setValues(RPGSystem *system, QList<int> *set, int type);
    void getValues(QList<int> *set);

    void clear();

private:
    Ui::CheckBoxList *ui;
};

#endif // CHECKBOXLIST_H
