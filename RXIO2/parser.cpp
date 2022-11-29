#include "parser.h"
#include "rxexception.h"

Parser::Parser(QJsonDocument *doc, QMap<QString, QStringList> *param_oders, QString file_location)
    : QObject()
{
    this->f.setFileName(file_location);
    f.open(QIODevice::ReadOnly);

    if (!f.exists())
        throw RXException("File not exist");

    QByteArray buffer = f.read(2);
    if (buffer.length() < 2)
        throw RXException("File too short");
    if (buffer.at(0) != 0x04 || buffer.at(1) != 0x08)
        throw RXException("wrong version in header");



    this->param_oders = param_oders;
    this->doc = doc;


    QJsonValue val = parse_token();
    if (val.isObject())
        doc->setObject(val.toObject());
    else if (val.isArray())
        doc->setArray(val.toArray());
    else
    {
        qDebug() << "TODO";
    }

    f.close();

}

QJsonValue Parser::parse_token()
{
    int byte = this->read_one_byte();
    if (byte == 0x5B) //[
    {
        int curr_obj_count = ++object_count;
        QJsonArray array;
        int len = this->read_fixnum();

        for (int i = 0; i < len; i++)
        {
            array.append(this->parse_token());
        }
        this->reference_table.insert(curr_obj_count, array);
        return QJsonValue(array);
    }
    else if (byte == 0x7B) //{ dictionary
    {
        int curr_obj_count = ++object_count;
        QJsonObject obj;
        int len = this->read_fixnum();
        obj.insert("RXClass", "dictionary");

        for (int i = 0; i < len; i++)
        {
            int key = this->parse_token().toInt(-1); //should be integer
            QJsonValue val = this->parse_token(); // object

            obj.insert(QString::number(key), val);
        }
        this->reference_table.insert(curr_obj_count, obj);
        return QJsonValue(obj);
    }
    else if (byte == 0x30)
    {
        return QJsonValue();
    }
    else if (byte == 0x46) //F false
    {
        return QJsonValue(false);
    }
    else if (byte == 0x54) //T true
    {
        return QJsonValue(true);
    }
    else if (byte == 0x22) //" string
    {
        int len = this->read_fixnum();
        QJsonValue str(QString(this->f.read(len)));
        this->reference_table.insert(++object_count, str);
        return str;
    }
    else if (byte == 0x49) //I" string coded differently
    {
        qDebug() << "get strings with encoding right" << f.pos();
        exit(1234);
        int len = this->read_fixnum();
        QJsonValue str(QString(this->f.read(len)));
        this->reference_table.insert(++object_count, str);
        return str;
    }
    else if (byte == 0x69) //i integer
    {
        return QJsonValue(this->read_fixnum());
    }
    else if (byte == 0x75) //u user defined
    {
        QString symbol = this->read_symbol_or_link();
        int curr_obj_count = ++object_count;

        if (symbol == "Table")
        {
            int size = this->read_fixnum(); //size in bytes
            int end_pos = this->f.pos()+size;
            int shitty_num = this->read_32_bit();
            int x = this->read_32_bit();
            int y = this->read_32_bit();
            int z = this->read_32_bit();
            int num_elements = this->read_32_bit();

            if (x*y*z != num_elements)
                throw RXException("Invalid Table");


            QJsonObject table;
            table.insert("RXClass", "Table");
            table.insert("shitty_num", QJsonValue(shitty_num));
            table.insert("x", QJsonValue(x));
            table.insert("y", QJsonValue(y));
            table.insert("z", QJsonValue(z));

            QJsonArray values;

            for (int i = 0; i < num_elements; i++)
            {
                values.append(QJsonValue(this->read_16_bit()));
            }
            table.insert("values", values);


            f.seek(end_pos); //in case the table is messed up
            this->reference_table.insert(curr_obj_count, table);
            return QJsonValue(table);
        }
        else if (symbol == "Color" || symbol == "Tone")
        {
            double r,g,b, alpha;

            this->read_fixnum(); //32 byte size, ignore it

            this->f.read((char*)&r, 8);
            this->f.read((char*)&g, 8);
            this->f.read((char*)&b, 8);
            this->f.read((char*)&alpha, 8);

            QJsonObject obj;
            obj.insert("RXClass", symbol);
            obj.insert("r", r);
            obj.insert("g", g);
            obj.insert("b", b);
            obj.insert("alpha_gray", alpha);
            this->reference_table.insert(curr_obj_count, obj);
            return QJsonValue(obj);
        }

    }
    else if (byte == 0x6F) //o obj
    {
        int curr_obj_count = ++object_count;
        QJsonObject obj;
        QString symbol = this->read_symbol_or_link();
        int attribute_count = this->read_fixnum();


        QStringList param_oder;

        obj.insert("RXClass", symbol);
        for (int i = 0; i < attribute_count; i++)
        {
            QString attribute_symbol = this->read_symbol_or_link();
            param_oder << attribute_symbol;
            QJsonValue attribute_value = this->parse_token();

            obj.insert(attribute_symbol, attribute_value);
        }
        if (!param_oders->contains(symbol))
            param_oders->insert(symbol, param_oder);

        this->reference_table.insert(curr_obj_count, obj);
        return QJsonValue(obj);
    }
    else if (byte == 0x40)
    {
        int ref = this->read_fixnum();
        if (this->reference_table.contains(ref))
            return this->reference_table.value(ref);
        else
        {
            throw RXException("Undefined Reference: " + QString::number(ref) + " " + QString::number(reference_table.size()) + " " + QString::number(f.pos(),16));
        }
    }



    return QJsonValue(-1);
}

int Parser::read_one_byte()
{
    QByteArray one_byte = f.read(1);
    if (one_byte.length() != 1)
    {
        throw RXException("Unexpected EOF");
    }
    return 0xFF & (one_byte.at(0));
}

int Parser::read_fixnum()
{
    int num = f.read(1).at(0);

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

        if (num == -1)
        {
            int tmp = this->read_one_byte();
            if (tmp <= -124 && tmp >= -128) return tmp;
            else //if (tmp >= 0 && tmp <= 127)
                return -(129 + 127 - tmp);
        }
        else if (num == -2)
        {
            int tmp = this->read_one_byte() & 0xFF;
            tmp |= ((this->read_one_byte()) & 0xFF) << 8;

            return -257 - (0xfeff-tmp);
        }

        return num;
    }

    return num;
}

int Parser::read_16_bit()
{
    int v = 0;
    v = this->read_one_byte();
    v |= (this->read_one_byte() << 8);

    return v;
}

int Parser::read_32_bit()
{
    int v = 0;
    v = this->read_one_byte();
    v |= (this->read_one_byte() << 8);
    v |= (this->read_one_byte() << 16);
    v |= (this->read_one_byte() << 24);

    return v;
}


QString Parser::read_symbol_or_link(bool save_symbol)
{
    QString symbol;

    int current_byte = this->read_one_byte();
    if (current_byte == ':')
    {
        int symbol_len = this->read_fixnum();
        symbol = QString(this->f.read(symbol_len));
        if (save_symbol)
            this->symbol_cache << symbol;
    }
    else if (current_byte == ';')
    {
        int symbol_index = this->read_fixnum();
        symbol = this->symbol_cache.at(symbol_index);
    }
    else
    {
        throw RXException("Symbol or link expected: " + QString::number(this->f.pos(),16).toUpper());
    }

    return symbol;
}
