#ifndef TRAINEREDITOR_H
#define TRAINEREDITOR_H

#include <QWidget>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonValue>
#include <QListWidgetItem>

class RPGDB;

namespace Ui {
class TrainerEditor;
}

class TrainerEditor : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerEditor(RPGDB *db, QWidget *parent = nullptr);
    ~TrainerEditor();

public slots:
    void read_trainers();

private slots:
    void on_list_trainers_currentRowChanged(int currentRow);


    void on_button_change_type_clicked();

private:
    Ui::TrainerEditor *ui;
    RPGDB *db;
    QJsonDocument trainers_file;
    QJsonDocument trainer_types_file;
};

#endif // TRAINEREDITOR_H
