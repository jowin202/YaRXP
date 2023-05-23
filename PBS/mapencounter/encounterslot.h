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

    QString get_name();
    int get_prob();
    int get_lv_from();
    int get_lv_to();

signals:
    void content_changed();
    void slot_deleted();

private slots:
    void on_button_delete_clicked();

private:
    Ui::EncounterSlot *ui;
};

#endif // ENCOUNTERSLOT_H
