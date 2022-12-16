#ifndef SWITCHVARIABLEWIDGET_H
#define SWITCHVARIABLEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QComboBox>

namespace Ui {
class SwitchVariableWidget;
}

class RPGDB;
class SwitchVariableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchVariableWidget(QWidget *parent = nullptr);
    ~SwitchVariableWidget();

    void setSwitchWidget(RPGDB *db);
    void setVariableWidget(RPGDB *db);


public slots:
    void setValue_from_dialog(int value);
    void setValue(int value);
    int getValue();
    QComboBox *get_combo();

private slots:
    void on_toolButton_clicked();

private:
    Ui::SwitchVariableWidget *ui;
    RPGDB *db;

    bool switch_widget = false;
    bool variable_widget = false;
};

#endif // SWITCHVARIABLEWIDGET_H
