#ifndef ENCOUNTERSLOT_H
#define ENCOUNTERSLOT_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

namespace Ui {
class EncounterSlot;
}

class EncounterSlot : public QWidget
{
    Q_OBJECT

public:
    explicit EncounterSlot(QJsonArray data, QWidget *parent = nullptr);
    ~EncounterSlot();

    QJsonArray get_data();


signals:
    void content_changed(QJsonArray data);
    void slot_deleted();

private slots:
    void on_button_delete_clicked();

private:
    Ui::EncounterSlot *ui;
};

#endif // ENCOUNTERSLOT_H
