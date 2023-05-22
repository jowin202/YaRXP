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

private slots:
    void update_slots();

signals:
    void data_changed(QJsonArray data);

private:
    Ui::EncounterWidget *ui;
    QJsonArray encounters_data;
    int index;
};

#endif // ENCOUNTERWIDGET_H
