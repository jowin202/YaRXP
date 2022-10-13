#ifndef ABCLIST_H
#define ABCLIST_H

#include <QWidget>

namespace Ui {
class ABCList;
}

class RPGSystem;

class ABCList : public QWidget
{
    Q_OBJECT

public:
    explicit ABCList(QWidget *parent = nullptr);
    ~ABCList();

    void setStates(RPGSystem *system, QList<int> *ranks);
    void setElements(RPGSystem *system, QList<int> *ranks);

    void getValues(QList<int> *ranks);
    void clear();

private:
    Ui::ABCList *ui;
};

#endif // ABCLIST_H
