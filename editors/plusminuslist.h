#ifndef PLUSMINUSLIST_H
#define PLUSMINUSLIST_H

#include <QWidget>
#include <QJsonArray>

namespace Ui {
class PlusMinusList;
}


class PlusMinusList : public QWidget
{
    Q_OBJECT

public:
    explicit PlusMinusList(QWidget *parent = nullptr);
    ~PlusMinusList();


    void add_box(QString name, int val);
    QJsonArray get_plus();
    QJsonArray get_minus();

    void clear();


signals:
    void values_changed();

private:
    Ui::PlusMinusList *ui;
};

#endif // PLUSMINUSLIST_H
