#include "undodb.h"

UndoDB::UndoDB(QObject *parent)
    : QObject{parent}
{

}

void UndoDB::push(int map, QJsonObject obj)
{
    if (map < 0 || map > 999) return;
    this->maps[map].push(obj);
}

QJsonObject UndoDB::pop(int map)
{
    if (map < 0 || map > 999) return QJsonObject();
    if (this->maps[map].isEmpty()) return QJsonObject();
    return this->maps[map].pop();
}
