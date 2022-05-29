#ifndef RXDATAWRITER_H
#define RXDATAWRITER_H

#include <QObject>

class RXDataWriter : public QObject
{
    Q_OBJECT
public:
    explicit RXDataWriter(QObject *parent = nullptr);

signals:

};

#endif // RXDATAWRITER_H
