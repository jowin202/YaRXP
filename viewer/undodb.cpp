#include "undodb.h"

UndoDB::UndoDB(QObject *parent)
    : QObject{parent}
{

}

void UndoDB::push(int map, QJsonObject obj, bool isFirst)
{
    if (map < 0 || map > 999) return;
    // If the stack is empty, push the object. If the stack is not empty, only push if the new object is different from the last one to prevent duplicates
    else if ((isFirst && this->maps[map].isEmpty()) || (!isFirst && !this->maps[map].isEmpty() && this->maps[map].last() != obj))
    {
        this->maps[map].push(obj);
    }
}

QJsonObject UndoDB::pop(int map)
{
    if (map < 0 || map > 999) return QJsonObject();
    if (this->maps[map].isEmpty()) return QJsonObject();
    // If there is only one element, return it without popping to prevent emptying the stack
    if (this->maps[map].size() == 1)
        return this->maps[map].first();
    return this->maps[map].pop();
}
