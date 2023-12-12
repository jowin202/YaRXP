#include "searchthread.h"

#include "rpgdb.h"

SearchThread::SearchThread(RPGDB *db, int mode, QString text, int sw_var, Qt::CaseSensitivity cs)
    : QThread{}
{
    this->db = db;
    this->mode = mode;
    this->text = text;
    this->sw_var = sw_var;
    this->case_sensitive_options = cs;
}

void SearchThread::run()
{
    QJsonDocument *doc;
    QJsonObject map;
    QJsonObject events;
    QJsonObject event;
    QJsonArray pages;
    QJsonObject page;
    QJsonArray list;
    QJsonObject cmd;
    QJsonArray param;
    int code = 0;
    bool ok = true;
    for (int id = 0; id < 1000; id++)
    {
        if ((doc = db->get_mapfile_by_id(id)) != 0)
        {
            map = doc->object();
            events = map.value("@events").toObject();

            //iterate over map events
            foreach (const QString& key, events.keys())
            {
                if (key == "RXClass") continue; //find all events but skip text argument
                key.toInt(&ok,10);
                if (ok)
                {
                    event = events.value(key).toObject();
                    pages = event.value("@pages").toArray();
                    for (int i = 0; i < pages.count(); i++)
                    {
                        page = pages.at(i).toObject();
                        list = page.value("@list").toArray();

                        for (int j = 0; j < list.count(); j++)
                        {
                            cmd = list.at(j).toObject();
                            param = cmd.value("@parameters").toArray();
                            code = cmd.value("@code").toInt();
                            if (mode == TEXT)
                            {
                                for (int k = 0; k < param.count(); k++)
                                {
                                    if (param.at(k).isString() && param.at(k).toString().contains(text, case_sensitive_options))
                                    {
                                        //qDebug() << id << event.value("@id").toInt() << (i+1) << j;
                                        result << id;
                                        result << event.value("@id").toInt();
                                        result << i;
                                        result << j;
                                    }
                                }
                            }
                            else if (mode == SWITCH &&
                                     (code == 121 && param.at(0).toInt() <= sw_var && param.at(1).toInt() >= sw_var)
                                     )//TODO
                            {
                                //qDebug() << "switch" << id << event.value("@id").toInt() << i << j << param;
                                result << id;
                                result << event.value("@id").toInt();
                                result << i;
                                result << j;
                            }
                            else if (mode == VARIABLE &&
                                     (code == 122 && param.at(0).toInt() <= sw_var && param.at(1).toInt() >= sw_var)
                                     )//TODO
                            {
                                //qDebug() << "variable" << id << event.value("@id").toInt() << i << j << param;
                                result << id;
                                result << event.value("@id").toInt();
                                result << i;
                                result << j;
                            }
                        }

                    }

                }
            }
        }
    }
}
