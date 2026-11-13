#include "writer.h"
#include "rxexception.h"

Writer::Writer(QJsonDocument *doc, QString file_location, bool strings_from_base_64, bool extended_dictionary)
    : QObject()
{
    this->strings_from_base_64 = strings_from_base_64;
    this->extended_dictionary = extended_dictionary;
    if (file_location.isEmpty())
        throw RXException("No file name given for writing");

    //QSaveFile writes to a temporary file and replaces the target only on commit(),
    //so the original file stays intact if anything goes wrong while writing
    this->f.setFileName(file_location);
    if (!f.open(QIODevice::WriteOnly))
        throw RXException("Could not open " + file_location + " for writing: " + f.errorString());

    this->write_one_byte(0x04);
    this->write_one_byte(0x08);


    this->doc = doc;


    if (doc->isArray())
        this->write_token(doc->array());
    else if (doc->isObject())
        this->write_token(doc->object());
    else
        throw RXException("Nothing to write for " + file_location);

    if (!f.commit())
        throw RXException("Could not write " + file_location + ": " + f.errorString());
}

void Writer::write_token(QJsonValue token)
{
    if (token.isArray())
    {
        const QJsonArray array = token.toArray();
        this->write_one_byte(0x5B);
        this->write_fixnum(array.size());

        for (const QJsonValue &value : array)
            this->write_token(value);
    }
    else if (token.isObject() && token.toObject().contains("RXClass"))
    {
        QJsonObject obj = token.toObject();

        if (obj.value("RXClass") == "dictionary")
        {

            if (extended_dictionary)
            {
                QJsonArray dict_list = obj.value("dict_list").toArray();

                //only key-value pairs can be written, the count must match the written entries
                QList<QJsonArray> pairs;
                for (const QJsonValue &entry : dict_list)
                    if (entry.toArray().count() == 2)
                        pairs.append(entry.toArray());

                //do extended dictionary stuff
                this->write_one_byte(0x7B);
                this->write_fixnum(pairs.count());

                for (const QJsonArray &pair : pairs)
                {
                    this->write_token(pair.at(0));
                    this->write_token(pair.at(1));
                }
            }
            else
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
        else if (obj.value("RXClass") == "symbol")
        {
            Q_ASSERT(this->extended_dictionary);
            //This should only happen when extended_dictionary is true
            //Only for PBS
            this->write_symbol_or_link(obj.value("symbol_value").toString());
        }
        else //normal object
        {
            int size = obj.size()-1; //for RXClass
            QString obj_type = obj.value("RXClass").toString();
            this->write_one_byte(0x6F);
            this->write_symbol_or_link(obj_type);
            this->write_fixnum(size);


            foreach(const QString& key, obj.keys()) {
                if (key == "RXClass") continue;
                this->write_symbol_or_link(key);
                this->write_token(obj.value(key));
            }
        }
    }
    else if (token.isDouble())
    {
        //it is actually integer
        this->write_integer(token.toInteger());
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
    else
    {
        //writing nothing would corrupt the whole file (counts would not match anymore)
        throw RXException("Cannot write value (object without RXClass or undefined value) to " + f.fileName());
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
    auto it = symbol_cache.constFind(str);
    if (it != symbol_cache.constEnd())
    {
        write_one_byte((int)';');
        write_fixnum(it.value());
    }
    else
    {
        QByteArray bytes = str.toUtf8();
        write_one_byte((int)':');
        write_fixnum(bytes.length());
        f.write(bytes);
        int index = symbol_cache.size();
        this->symbol_cache.insert(str, index);
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
        else if (n >= -256)
        {
            this->write_one_byte(0xFF); //negative and 1 byte number
            this->write_one_byte(((-n-1) & 0xFF) ^ 0xFF);
        }
        else if (n >= -65536)
        {
            this->write_one_byte(0xFE); //negative and 2 byte number
            int offset = 0xFEFF -(-n-257);
            this->write_one_byte( 0xFF & offset);
            this->write_one_byte( 0xFF & (offset >> 8));
        }
        else if (n >= -16777216)
        {
            this->write_one_byte(0xFD); //negative and 3 byte number
            quint32 tmp = (quint32)(n + 0x1000000);
            this->write_one_byte(tmp & 0xFF);
            this->write_one_byte((tmp >> 8) & 0xFF);
            this->write_one_byte((tmp >> 16) & 0xFF);
        }
        else
        {
            this->write_one_byte(0xFC); //negative and 4 byte number
            quint32 tmp = (quint32)n; //full 32 bit two's complement pattern
            this->write_one_byte(tmp & 0xFF);
            this->write_one_byte((tmp >> 8) & 0xFF);
            this->write_one_byte((tmp >> 16) & 0xFF);
            this->write_one_byte((tmp >> 24) & 0xFF);
        }
    }

}

void Writer::write_integer(qint64 n)
{
    //Fixnums in RGSS1 (Ruby 1.8, 32 bit) are 31 bit wide, everything else is a Bignum
    if (n < -(1LL << 30) || n >= (1LL << 30))
    {
        this->write_bignum(n);
        return;
    }
    write_one_byte((int)'i');
    write_fixnum((int)n);
}

void Writer::write_bignum(qint64 n)
{
    write_one_byte((int)'l');
    write_one_byte(n < 0 ? '-' : '+');

    quint64 abs_value = (n < 0) ? (quint64)(-(n + 1)) + 1 : (quint64)n;
    QByteArray bytes;
    while (abs_value > 0)
    {
        bytes.append((char)(abs_value & 0xFF));
        abs_value >>= 8;
    }
    if (bytes.length() % 2 == 1)
        bytes.append((char)0); //length is given in 16 bit words

    write_fixnum(bytes.length() / 2);
    f.write(bytes);
}

void Writer::write_string(QString str)
{
    write_one_byte((int)'"');
    if (strings_from_base_64)
    {
        QByteArray output = QByteArray::fromBase64(str.toUtf8());
        write_fixnum(output.length());
        f.write(output);
    }
    else
    {
        QByteArray bytes = str.toUtf8();
        write_fixnum(bytes.length());
        f.write(bytes);
    }
}
