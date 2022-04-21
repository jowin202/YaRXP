#include "rxdataparser.h"

RXDataParser::RXDataParser(QString file) : QObject()
{
    this->filePath = file;
    this->file.setFileName(file); //absolute path
    this->file.open(QIODevice::ReadOnly);
}

int RXDataParser::check_version()
{
    if (this->file.exists())
    {
        QByteArray buffer = file.read(2);
        if (buffer.length() < 2)
        {
            qDebug() << "too short";
            return -2;
        }
        if (buffer.at(0) != 0x04 || buffer.at(1) != 0x08)
        {
            qDebug() << "wrong version";
            return -1;
        }
        return 0;
    }
    else
        return -3;
}

int RXDataParser::get_byte_and_rev()
{
    quint64 pos = this->file.pos();
    int val = this->read_one_byte();
    this->file.seek(pos);
    return val;
}

int RXDataParser::read_fixnum()
{
    int num = this->read_one_byte();

    if (num == 0)
        return num;

    if (num > 0)
    {
        if (4 < num && num < 128) return num - 5;
        quint32 x = 0;
        for (int i = 0; i < 4; i++)
        {
            x = (i < num ? this->read_one_byte() << 24 : 0) | (x >> 8);
        }
        return (int)x;
    }
    else{
        if (-129 < num && num < -4) return num + 5;
        num = -num;

        int x = -1;
        const quint32 mask = ~(0xff << 24);
        for (int i = 0; i < 4; i++)
            x = (i < num ? this->read_one_byte() << 24 : 0xff) | ((x >> 8) & mask);
        return x;
    }

    return num;
}

int RXDataParser::read_one_byte()
{
    QByteArray one_byte = file.read(1);
    if (one_byte.length() != 1)
    {
        qDebug() << "weird error";
        return -1;
    }
    return (one_byte.at(0));
}

void RXDataParser::parse()
{
    if (this->file.exists())
    {
        QByteArray buffer = file.read(2);
        if (buffer.length() < 2)
        {
            qDebug() << "too short";
            return;
        }
        if (buffer.at(0) != 0x04 || buffer.at(1) != 0x08)
        {
            qDebug() << "wrong version";
            return;
        }

        int type = this->read_one_byte();

        if (type == '@')
        {

        }
        else if(type == 'I')
        {

        }
        else if (type == 'o')
        {
            //object
        }
        else if (type == ':')
        {
            //string
        }

        //file.close();
    }
}

void RXDataParser::parseMapInfo(QList<MapTreeWidgetItem*> *map_list)
{
    this->symbol_cache.clear();
    if (this->check_version() < 0) //file exists, version correct
    {
        qDebug() << "error";
        return;
    }

    if (this->read_one_byte() != 0x7b)
    {
        qDebug() << "byte 7b expected";
        return;
    }
    int length = this->read_fixnum(); //expected: 0x7b length key value key value key value ...

    int current_byte;
    int symbol_index;
    int key;
    int symbol_len;
    int name_len;
    int attribute_count;
    QString current_symbol;
    QString current_name;

    for (int i = 0; i < length; i++) //browse all objects
    {
        map_list->append(new MapTreeWidgetItem());

        if (this->read_one_byte() == 'i') //get object id (key)
        {
            key = this->read_fixnum();
            map_list->last()->set_id(key);

            if (this->read_one_byte() == 'o')
            {
                current_byte = this->read_one_byte();
                if (current_byte == ':') //name of the object, should be "RPG::MapInfo" or the link
                {
                    symbol_len = this->read_fixnum();
                    this->symbol_cache << QString(this->file.read(symbol_len));
                }
                else if (current_byte == ';')
                {
                    symbol_index = this->read_fixnum(); // todo: check if it is RPG::MapInfo ..
                }
                //else error

                attribute_count = this->read_fixnum(); //should always be 6
                for (int j = 0; j < attribute_count; j++)
                {
                    current_byte = this->read_one_byte();
                    if (current_byte == ':') //symbol
                    {
                        symbol_len = this->read_fixnum();
                        current_symbol = QString(this->file.read(symbol_len));
                        this->symbol_cache << current_symbol;
                    }
                    else if (current_byte == ';')
                    {
                        symbol_index = this->read_fixnum();
                        current_symbol = symbol_cache.at(symbol_index);
                    }

                    if (current_symbol == "@scroll_x" || current_symbol == "@scroll_y"  || current_symbol == "@parent_id" || current_symbol == "@order") //value
                    {
                        if (this->read_one_byte() == 'i') //must be integer
                        {
                            map_list->last()->setParameter(current_symbol, this->read_fixnum());
                        }
                    }
                    else if (current_symbol == "@name")
                    {
                        if (this->read_one_byte() == '"')
                        {
                            name_len = this->read_fixnum();
                            current_name = this->file.read(name_len);
                            map_list->last()->setName(current_name);
                        }
                        else
                        {
                            qDebug() << "string expected for name";
                            return;
                        }
                    }
                    else if (current_symbol == "@expanded")
                    {
                        if (this->read_one_byte() == 'F')
                            map_list->last()->setExpanded(false);
                        else map_list->last()->setExpanded(true);
                    }

                }

            }
            else
            {
                qDebug() << "object as value expected";
                return;
            }
        }
        else
        {
            qDebug() << "integer as key expected";
            return;
        }
    }


}

bool RXDataParser::parseBool()
{
    int val = this->read_one_byte();
    if (val == 'T')
    {
        return true;
    }
    if (val == 'F')
    {
        return false;
    }
    qDebug() << "error: bool expected";
    return false;
}
