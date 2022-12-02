#ifndef CHECKBOXLIST_H
#define CHECKBOXLIST_H

#include <QWidget>
#include <QJsonArray>

namespace Ui {
class CheckBoxList;
}

class CheckBoxList : public QWidget
{
    Q_OBJECT

public:
    explicit CheckBoxList(QWidget *parent = nullptr);
    ~CheckBoxList();

    void add_checkbox(QString name, bool is_checked);
    QJsonArray get_result();

    void clear();

signals:
    void values_changed();

private:
    Ui::CheckBoxList *ui;
};

#endif // CHECKBOXLIST_H
