#ifndef RPGCLASS_H
#define RPGCLASS_H

#include <QObject>

class RPGClass : public QObject
{
    Q_OBJECT
public:
    explicit RPGClass(QObject *parent = nullptr);

signals:

};

#endif // RPGCLASS_H
