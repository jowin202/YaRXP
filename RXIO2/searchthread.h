#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QObject>
#include <QList>
#include <QCoreApplication>

class RPGDB;

class SearchThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchThread(RPGDB *db, int mode, QString text, int sw_var, Qt::CaseSensitivity cs);

    enum SearchMode { TEXT, SWITCH, VARIABLE, COMMAND };
    void run();

    RPGDB *db;
    int mode;
    QString text;
    int sw_var;
    Qt::CaseSensitivity case_sensitive_options;

    QList<int> result;


signals:
    void finished(QList<int>);
};

#endif // SEARCHTHREAD_H
