#ifndef ENCOUNTERWIDGET_H
#define ENCOUNTERWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>
#include <QMessageBox>

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

    void on_button_remove_encounter_clicked();

signals:
    void data_changed();
    void encounter_widget_deleted();

private:
    Ui::EncounterWidget *ui;
    QJsonArray encounters_data;
    int index;
};

#endif // ENCOUNTERWIDGET_H
