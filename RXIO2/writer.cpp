#include "writer.h"
#include "rxexception.h"

Writer::Writer(QJsonDocument *doc, QMap<QString, QStringList> *param_oders, QString file_location)
    : QObject()
{
    this->f.setFileName(file_location);
    f.open(QIODevice::WriteOnly);

    this->write_one_byte(0x04);
    this->write_one_byte(0x08);


    this->param_orders = param_oders;
    this->doc = doc;


    if (doc->isArray())
        this->write_token(doc->array());
    else if (doc->isObject())
        this->write_token(doc->object());

    f.close();
}

void Writer::write_token(QJsonValue token)
{
    //TODO
    if (token.isArray())
    {
        this->write_one_byte(0x5B);
        int len = token.toArray().size();
        this->write_fixnum(len);

        for (int i = 0; i < len; i++)
        {
            this->write_token(token.toArray().at(i));
        }
    }
    else if (token.isObject() && token.toObject().contains("RXClass"))
    {
        QJsonObject obj = token.toObject();

        if (obj.value("RXClass") == "dictionary")
        {
            //do dictionary stuff
            this->write_one_byte(0x7B);
            this->write_fixnum(obj.size()-1); //excluding rxclass

            foreach(const QString& key, obj.keys()) {
                if (key == "RXClass") continue;
                this->write_integer(key.toInt()); //keys are always integer, needed for MapInfos
                this->write_token(obj.value(key));
            }


        }
        else if (obj.value("RXClass") == "Table")
        {
            //do table stuff
            this->write_one_byte('u');
            this->write_symbol_or_link("Table");
            int num_elements = obj.value("x").toInt() * obj.value("y").toInt() * obj.value("z").toInt();
            int size =  num_elements * 2;
            size += 20; //4 bytes for x,y,z,shitty num, num_elements
            this->write_fixnum(size);
            this->write_32_bit(obj.value("shitty_num").toInt());
            this->write_32_bit(obj.value("x").toInt());
            this->write_32_bit(obj.value("y").toInt());
            this->write_32_bit(obj.value("z").toInt());
            this->write_32_bit(num_elements);

            QJsonArray values = obj.value("values").toArray();

            for (int i = 0; i < num_elements; i++)
            {
                this->write_one_byte(values.at(i).toInt() & 0xFF);
                this->write_one_byte((values.at(i).toInt()>>8) & 0xFF);
            }

        }
        else if (obj.value("RXClass") == "Color" || obj.value("RXClass") == "Tone")
        {
            //do color stuff
            this->write_one_byte('u');
            this->write_symbol_or_link(obj.value("RXClass").toString());
            double r = obj.value("r").toDouble();
            double g = obj.value("g").toDouble();
            double b = obj.value("b").toDouble();
            double alpha = obj.value("alpha_gray").toDouble();

            this->write_fixnum(32); //size

            this->f.write((char*)&r, 8);
            this->f.write((char*)&g, 8);
            this->f.write((char*)&b, 8);
            this->f.write((char*)&alpha, 8);
        }
        else //normal object
        {
            int size = obj.size()-1; //for RXClass
            QString obj_type = obj.value("RXClass").toString();
            this->write_one_byte(0x6F);
            this->write_symbol_or_link(obj_type);
            this->write_fixnum(size);

            if (!param_orders->contains(obj_type) || param_orders->value(obj_type).length() != size)
            {
                qDebug() << obj_type << size;
                throw RXException("object attribute size failed");
            }
            QStringList params = param_orders->value(obj_type);

            for (int i = 0; i < size; i++)
            {
                this->write_symbol_or_link(params.at(i));
                if (!obj.contains(params.at(i)))
                    throw RXException("Object of Class " + obj_type + " does not contain attribute " + params.at(i));
                this->write_token(obj.value(params.at(i)));
            }
        }
    }
    else if (token.isDouble())
    {
        //it is actually integer
        this->write_integer(token.toInt());
    }
    else if (token.type() == QJsonValue::Type::Bool)
    {
        if (token.toBool())
            this->write_one_byte('T');
        else
            this->write_one_byte('F');
    }
    else if (token.type() == QJsonValue::Type::Null)
    {
        this->write_one_byte(0x30);
    }
    else if (token.type() == QJsonValue::Type::String)
    {
        this->write_string(token.toString());
    }
}

void Writer::write_32_bit(int n)
{
    this->write_one_byte(n & 0xFF);
    this->write_one_byte((n>>8) & 0xFF);
    this->write_one_byte((n>>16) & 0xFF);
    this->write_one_byte((n>>24) & 0xFF);
}

void Writer::write_symbol_or_link(QString str)
{
    if (symbol_cache.contains(str))
    {
        write_one_byte((int)';');
        write_fixnum(symbol_cache.indexOf(str));
    }
    else
    {
        write_one_byte((int)':');
        write_fixnum(str.length());
        f.write(str.toUtf8());
        this->symbol_cache << str;
    }
}

void Writer::write_one_byte(int byte)
{
    f.write(QByteArray(1,(char)byte));
}

void Writer::write_fixnum(int n)
{
    if (n == 0)
        write_one_byte(0x00);
    else if (n > 0)
    {
        if (0 < n && n < 123)
            write_one_byte(n+5);
        else //32 bit
        {
            if ((n >> 8) == 0)
            {
                write_one_byte(0x01);
                write_one_byte(n & 0xFF);
            }
            else if ((n >> 16) == 0)
            {
                write_one_byte(0x02);
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
            }
            else if ((n >> 24) == 0)
            {
                write_one_byte(0x03);
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
            }
            else
            {
                write_one_byte(0x04);
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
            }
        }
    }
    else { // negative
        if (n < 0 && n > -124)
            write_one_byte(n-5);
        else
        {
            if (n <= -124 && n >= -256)
            {
                this->write_one_byte(0xFF); //negative and 1 byte number
                this->write_one_byte(((-n-1) & 0xFF) ^ 0xFF);

            }
            else
            {
                this->write_one_byte(0xFE); //negative and 2 byte number
                int offset = 0xFEFF -(-n-257);
                this->write_one_byte( 0xFF & offset);
                this->write_one_byte( 0xFF & (offset >> 8));

                //assuming -65535 is min (TODO, maybe)

            }
        }
    }

}

void Writer::write_integer(int n)
{
    write_one_byte((int)'i');
    write_fixnum(n);
}

void Writer::write_string(QString str)
{
    write_one_byte((int)'"');
    write_fixnum(str.toUtf8().length());
    f.write(str.toUtf8());
}
