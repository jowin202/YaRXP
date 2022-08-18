#ifndef TESTCASES_H
#define TESTCASES_H

#include <QObject>
#include "RXObjects/rpgsystem.h"

class Testcases : public QObject
{
    Q_OBJECT
public:
    explicit Testcases(QObject *parent = nullptr);
    Testcases(RPGSystem *settings);

    bool ok;
signals:

};

#endif // TESTCASES_H
