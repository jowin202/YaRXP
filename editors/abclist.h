#ifndef ABCLIST_H
#define ABCLIST_H

#include <QWidget>
#include <QJsonArray>

namespace Ui {
class ABCList;
}

class ABCList : public QWidget
{
    Q_OBJECT

public:
    explicit ABCList(QWidget *parent = nullptr);
    ~ABCList();

    void add_box(QString name, int value);
    QJsonArray get_result();
    void clear();


signals:
    void values_changed();

private:
    Ui::ABCList *ui;
};

#endif // ABCLIST_H
