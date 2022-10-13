#ifndef PLUSMINUSLIST_H
#define PLUSMINUSLIST_H

#include <QWidget>

namespace Ui {
class PlusMinusList;
}

class RPGSystem;

class PlusMinusList : public QWidget
{
    Q_OBJECT

public:
    explicit PlusMinusList(QWidget *parent = nullptr);
    ~PlusMinusList();

    void setStates(RPGSystem *system, QList<int> *plus_states, QList<int> *minus_states);
    void getStates(QList<int> *plus_states, QList<int> *minus_states);
private:
    Ui::PlusMinusList *ui;
};

#endif // PLUSMINUSLIST_H
