#include "undodb.h"

UndoDB::UndoDB(QObject *parent)
    : QObject{parent}
{

}

void UndoDB::push(int map, QJsonObject obj, bool isFirst)
{
    if (map < 0) return;
    QStack<QJsonObject> &stack = this->maps[map];
    // If the stack is empty, push the object. If the stack is not empty, only push if the new object is different from the last one to prevent duplicates
    if ((isFirst && stack.isEmpty()) || (!isFirst && !stack.isEmpty() && stack.last() != obj))
    {
        stack.push(obj);
        // keep the first element (the state when the map was loaded) and drop the oldest steps after it
        while (stack.size() > max_steps)
            stack.removeAt(1);
    }
}

QJsonObject UndoDB::pop(int map)
{
    auto it = this->maps.find(map);
    if (it == this->maps.end() || it->isEmpty()) return QJsonObject();
    // If there is only one element, return it without popping to prevent emptying the stack
    if (it->size() == 1)
        return it->first();
    return it->pop();
}
