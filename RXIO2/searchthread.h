#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QObject>

class RPGDB;

class SearchThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchThread(RPGDB *db, int mode, QString text, int var_sw_cmd, Qt::CaseSensitivity cs);

    enum SearchMode { TEXT, SWITCH, VARIABLE, COMMAND };
    void run();



    RPGDB *db;
    int mode;
    QString text;
    int var_sw_cmd;
    Qt::CaseSensitivity case_sensitive_options;
};

#endif // SEARCHTHREAD_H
