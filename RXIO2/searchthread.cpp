#include "searchthread.h"

#include "rpgdb.h"

SearchThread::SearchThread(RPGDB *db, int mode, QString text, int var_sw_cmd, Qt::CaseSensitivity cs)
    : QThread{}
{
    this->db = db;
    this->mode = mode;
    this->text = text;
    this->var_sw_cmd = var_sw_cmd;
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
    bool ok = true;
    for (int id = 0; id < 1000; id++)
    {
        if ((doc = db->get_mapfile_by_id(id)) != 0)
        {
            if (mode == TEXT)
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
                                for (int k = 0; k < param.count(); k++)
                                {
                                    if (param.at(k).isString() && param.at(k).toString().contains(text, case_sensitive_options))
                                    {
                                        qDebug() << id << event.value("@id").toInt() << (i+1) << j;
                                    }
                                }
                            }

                        }

                    }
                }
            }
        }
    }
}
