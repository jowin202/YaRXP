#ifndef ENCOUNTERWIDGET_H
#define ENCOUNTERWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

#include <QCryptographicHash>

namespace Ui {
class EncounterWidget;
}

class EncounterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EncounterWidget(QJsonArray encounters_data, int index, QWidget *parent = nullptr);
    ~EncounterWidget();

    QString type();
    int step_chance();
    QJsonArray add_slot_data(QJsonArray encounter);

private slots:
    void update_slots();


    void on_button_add_slot_clicked();

signals:
    void data_changed();

private:
    Ui::EncounterWidget *ui;
    QJsonArray encounters_data;
    int index;
};

#endif // ENCOUNTERWIDGET_H
