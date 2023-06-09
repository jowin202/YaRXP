#ifndef TRAINERPOKEMON_H
#define TRAINERPOKEMON_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>

namespace Ui {
class TrainerPokemon;
}

class TrainerPokemon : public QWidget
{
    Q_OBJECT

public:
    explicit TrainerPokemon(QJsonObject obj, QWidget *parent = nullptr);
    ~TrainerPokemon();

private:
    Ui::TrainerPokemon *ui;
};

#endif // TRAINERPOKEMON_H
