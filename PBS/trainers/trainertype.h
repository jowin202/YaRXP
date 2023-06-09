#ifndef TRAINERTYPE_H
#define TRAINERTYPE_H

#include <QWidget>
#include <QKeyEvent>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QCryptographicHash>

namespace Ui {
class TrainerType;
}

class TrainerType : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerType(QJsonArray type, QWidget *parent = nullptr);
    ~TrainerType();


    void keyPressEvent(QKeyEvent *e) {
        if(e->key() == Qt::Key_Escape)
            this->on_button_cancel_clicked();
        if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)
            this->on_button_ok_clicked();
    }

signals:
    void finished(QJsonArray);

private slots:
    void on_button_intro_bgm_clicked();
    void on_button_battle_bgm_clicked();
    void on_button_victory_bgm_clicked();

    void on_button_cancel_clicked();


    void on_button_ok_clicked();

private:
    Ui::TrainerType *ui;
};

#endif // TRAINERTYPE_H
