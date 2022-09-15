#ifndef SWITCHVARIABLEWIDGET_H
#define SWITCHVARIABLEWIDGET_H

#include <QWidget>
#include <QDebug>

namespace Ui {
class SwitchVariableWidget;
}

class RPGSystem;
class SwitchVariableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchVariableWidget(QWidget *parent = nullptr);
    ~SwitchVariableWidget();

    void setSwitchWidget(RPGSystem *system);
    void setVariableWidget(RPGSystem *system);


public slots:
    void setValue_from_dialog(int value);
    void setValue(int value);
    int getValue();

private slots:
    void on_toolButton_clicked();

private:
    Ui::SwitchVariableWidget *ui;
    RPGSystem *system;

    bool switch_widget = false;
    bool variable_widget = false;
};

#endif // SWITCHVARIABLEWIDGET_H
