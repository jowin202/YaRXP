#ifndef RXDATAPARSER_H
#define RXDATAPARSER_H

#include <QObject>
#include <QFile>
#include <QRegExp>
#include <QHash>

#include "maptreewidgetitem.h"
#include <QDebug>
#include "rxvariant.h"

class RXDataParser : public QObject
{
    Q_OBJECT
public:
    explicit RXDataParser(QString file = "");
    int check_version();
    int get_byte_and_rev();
    int read_fixnum();
    int read_one_byte();
    void parse();

    void parseMapInfo(QList<MapTreeWidgetItem *> *map_list);
    bool parseBool();

signals:

private:
    QString filePath;
    QFile file;
    QStringList symbol_cache;
    bool test = false;

};

#endif // RXDATAPARSER_H
