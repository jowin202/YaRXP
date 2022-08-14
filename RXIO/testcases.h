#ifndef TESTCASES_H
#define TESTCASES_H

#include <QObject>
#include "RXObjects/rpgsettings.h"

class Testcases : public QObject
{
    Q_OBJECT
public:
    explicit Testcases(QObject *parent = nullptr);
    Testcases(RPGSettings *settings);

signals:

};

#endif // TESTCASES_H
