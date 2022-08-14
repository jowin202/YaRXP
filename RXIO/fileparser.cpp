#include "fileparser.h"
#include "fileparser.h"
#include "fileparser.h"
#include "fileparser.h"

#include <QDebug>

FileParser::FileParser(QObject *parent) : QObject(parent)
{

}

ParserException *FileParser::getException(QString message)
{
    QVariantList list;
    list.append(this->file.fileName());
    list.append(this->file.pos());
    list.append(this->last_visited_function);


    return new ParserException(message, list);
}


void FileParser::check_header()
{
    this->last_visited_function = "check_header()";
    if (this->file.exists())
    {
        QByteArray buffer = file.read(2);
        if (buffer.length() < 2)
            throw getException("file too short");
        if (buffer.at(0) != 0x04 || buffer.at(1) != 0x08)
            throw getException("wrong version in header");
    }
    else
        throw getException("file does not exist");
}

void FileParser::write_header()
{
    this->last_visited_function = "write_header()";
    if (this->file.exists() && this->file.isWritable())
    {
        write_one_byte(0x04);
        write_one_byte(0x08);
    }
    else
        throw getException("file does not exist or is not writable");
}


int FileParser::read_one_byte()
{
    QByteArray one_byte = file.read(1);
    if (one_byte.length() != 1)
    {
        throw getException("unexpected eof");
    }
    return (one_byte.at(0));
}
void FileParser::write_one_byte(int byte)
{
    file.write(QByteArray(1,(char)byte));
}

int FileParser::look_one_byte_ahead()
{
    qint64 position = this->file.pos();
    QByteArray one_byte = this->file.read(1);
    if (one_byte.length() != 1)
    {
        throw getException("unexpected eof");
    }
    this->file.seek(position);
    return (one_byte.at(0));
}

QString FileParser::look_ahead_object_type()
{
    qint64 position = this->file.pos();
    QByteArray one_byte = this->file.read(1);
    if (one_byte.length() != 1)
    {
        throw getException("unexpected eof");
    }
    if (one_byte.at(0) != 'o')
    {
        throw getException("object expected");
    }
    QString object_type = this->read_symbol_or_link(false);
    this->file.seek(position);
    return object_type;
}

int FileParser::look_ahead_code()
{
    //TODO: still shitty. could also be a reference to code
    int code = -1;
    qint64 pos = this->file.pos();

    while (true)
    {
        while (this->read_one_byte() != '@');
        qint64 at_pos = this->file.pos();
        QByteArray code_str = this->file.read(4);
        if (code_str != "code")
            this->file.seek(at_pos+1);
        else
        {
            code = this->read_integer();
            this->file.seek(pos);
            return code;
        }
    }

    return code;
}


int FileParser::read_fixnum()
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

void FileParser::write_fixnum(int n)
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
                n >>= 8;
            }
            else if ((n >> 16) == 0)
            {
                write_one_byte(0x02);
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
                n >>= 8;
            }
            else if ((n >> 24) == 0)
            {
                write_one_byte(0x03);
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
                n >>= 8;
                write_one_byte(n & 0xFF);
                n >>= 8;
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
                n >>= 8;
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



QString FileParser::read_symbol_or_link(bool save_symbol)
{
    QString symbol;
    int current_byte = this->read_one_byte();
    if (current_byte == ':')
    {
        int symbol_len = this->read_fixnum();
        symbol = QString(this->file.read(symbol_len));
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
        throw getException("symbol or link expected: " + QString::number(this->file.pos(),16).toUpper());
    }

    return symbol;
}


void FileParser::write_symbol_or_link(QString str)
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
        file.write(str.toUtf8());
        this->symbol_cache << str;
    }
}


RPGString FileParser::read_string()
{
    int var = this->read_one_byte();
    if (var == '"')
    {
        int str_len = this->read_fixnum();
        return RPGString(this->file.read(str_len), false);
    }
    else if (var == 'I')
    {
        RPGString str = this->read_string();
        int val = this->read_fixnum(); //should be 1
        if (val != 1) //instance variable E=True is for encoding.
        {
            throw getException("instance var error weird"); //should never be reached
        }
        this->read_symbol_or_link(); //should be E
        this->read_bool(); //should be (T)rue
        return RPGString(str, true);
    }
    else
    {
        throw getException("String expected");
        return RPGString("");
    }
}

void FileParser::write_string(RPGString str)
{
    if (str.encoding)
        write_one_byte((int)'I');
    write_one_byte((int)'"');
    write_fixnum(str.toUtf8().length());
    file.write(str.toUtf8());
    if (str.encoding)
    {
        write_fixnum(1);
        write_symbol_or_link("E");
        write_one_byte((int)'T');
    }
    this->increase_object_count("string: " + str);
}



RPGVariant FileParser::read_variant() //string int or symbol or bool
{
    int read_byte = this->look_one_byte_ahead();
    if (read_byte == 'i')
        return RPGVariant(this->read_integer());
    else if (read_byte == '"')
        return RPGVariant(this->read_string());
    else if (read_byte == 'I') // Instance Variable
        return RPGVariant(this->read_string());
    else if (read_byte == 'T')
    {
        this->read_one_byte();
        return RPGVariant(true);
    }
    else if (read_byte == 'F')
    {
        this->read_one_byte();
        return RPGVariant(false);
    }
    return RPGVariant();
}

void FileParser::write_variant(RPGVariant var)
{
    if (var.rpgstring_set)
        this->write_string(var.str);
    else if (var.type() == QVariant::String)
        this->write_string(var.toString());
    else if (var.type() == QVariant::Int)
        this->write_integer(var.toInt());
    else if (var.type() == QVariant::Bool)
        this->write_bool(var.toBool());
}

int FileParser::read_integer()
{
    if (this->read_one_byte() == 'i') //get object id (key)
    {
        return this->read_fixnum();
    }
    else
    {
        throw getException("Integer expected");
    }
}

void FileParser::write_integer(int n)
{
    write_one_byte((int)'i');
    write_fixnum(n);
}

bool FileParser::read_bool()
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
    throw getException("Bool expected");
}

void FileParser::write_bool(bool val)
{
    if (val)
        write_one_byte((int)'T');
    else
        write_one_byte((int)'F');
}

int FileParser::read_list()
{
    if (this->read_one_byte() != 0x7b)
    {
        throw getException("list expected");
    }
    return read_fixnum(); //list length
}

void FileParser::write_list(int length)
{
    write_one_byte(0x7b);
    write_fixnum(length);
    this->increase_object_count("{}");
}


int FileParser::read_array()
{
    if (this->read_one_byte() != 0x5b)
    {
        throw getException("list expected");
    }
    return read_fixnum(); //list length
}

void FileParser::write_array(int length)
{
    write_one_byte(0x5b);
    write_fixnum(length);
    this->increase_object_count("[]");
}


QVariantList FileParser::read_object()
{
    if (this->read_one_byte() != 'o')
    {
        throw getException("Object expected");
    }


    QVariantList result;
    result.append(QVariant(read_symbol_or_link()));
    result.append(QVariant(read_fixnum()));

    return result;
}


void FileParser::write_object(QString symbol, int params)
{
    write_one_byte((int)'o');
    write_symbol_or_link(symbol);
    write_fixnum(params);
    this->increase_object_count("object + " + symbol);
}

void FileParser::read_table(QList<int> *list)
{
    if (read_one_byte() != 'u')
        throw getException("Table expected");
    if (read_symbol_or_link() != "Table")
        throw getException("Table expected");

    read_fixnum(); //ignore, size can be calculated, redundant

    //skip 4 bytes don't know why -.-
    for (int i = 0; i < 4; i++)
        this->read_one_byte();

    int x=0, y=0, z=0;

    for (int i = 0; i < 4; i++)
        x |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
    for (int i = 0; i < 4; i++)
        y |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
    for (int i = 0; i < 4; i++)
        z |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
    for (int i = 0; i < 4; i++)
        this->read_one_byte() ; //withdraw result, size can be calculated

    int size = x*y*z;

    for (int i = 0; i < size*2; i++)
        list->append(this->read_one_byte());
}

void FileParser::read_table_for_map(QList<int> *list)
{
    if (read_one_byte() != 'u')
        throw getException("Table expected");
    if (read_symbol_or_link() != "Table")
        throw getException("Table expected");

    read_fixnum(); //ignore, size can be calculated, redundant

    //skip 4 bytes don't know why -.-
    for (int i = 0; i < 4; i++)
        this->read_one_byte();

    int x=0, y=0, z=0;

    for (int i = 0; i < 4; i++)
        x |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
    for (int i = 0; i < 4; i++)
        y |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
    for (int i = 0; i < 4; i++)
        z |= ((this->read_one_byte() & 0xFF) << (8*i)) ;
    for (int i = 0; i < 4; i++)
        this->read_one_byte() ; //withdraw result, size can be calculated


    for (int i = 0; i < x*y*z; i++)
        list->append((this->read_one_byte()&0xFF) | ((this->read_one_byte() & 0xFF) << 8));
}

void FileParser::write_table(QList<int> *list)
{
    write_one_byte((int)'u');
    write_symbol_or_link("Table");

    this->write_fixnum(list->size() + 20); //not sure why 20 but works, maybe because of zero at beginning

    //dont know why doing this
    this->write_one_byte(1);
    this->write_one_byte(0);
    this->write_one_byte(0);
    this->write_one_byte(0);

    int x = list->size()/2;
    int y = 1;
    int z = 1;
    int xyz = x*y*z;

    for (int i = 0; i < 4; i++)
        this->write_one_byte( (x >> (8*i)) & 0xFF );

    for (int i = 0; i < 4; i++)
        this->write_one_byte( (y >> (8*i)) & 0xFF );

    for (int i = 0; i < 4; i++)
        this->write_one_byte( (z >> (8*i)) & 0xFF );

    for (int i = 0; i < 4; i++)
        this->write_one_byte( (xyz >> (8*i)) & 0xFF ); // same again redundancy

    for (int i = 0; i < list->size(); i++)
        this->write_one_byte(list->at(i));
}

void FileParser::write_table_for_map(QList<int> *list, int height, int width)
{
    write_one_byte((int)'u');
    write_symbol_or_link("Table");

    int x = width;
    int y = height;
    int z = 3;
    int size = x*y*z;

    this->write_fixnum(2*size + 20); //not sure why 20 but works, maybe because of zero at beginning

    //dont know why doing this
    this->write_one_byte(3); // 3 for maps (check if every map has 3 here)
    this->write_one_byte(0);
    this->write_one_byte(0);
    this->write_one_byte(0);


    for (int i = 0; i < 4; i++)
        this->write_one_byte( (x >> (8*i)) & 0xFF );

    for (int i = 0; i < 4; i++)
        this->write_one_byte( (y >> (8*i)) & 0xFF );

    for (int i = 0; i < 4; i++)
        this->write_one_byte( (z >> (8*i)) & 0xFF );

    for (int i = 0; i < 4; i++)
        this->write_one_byte( (size >> (8*i)) & 0xFF ); // same again redundancy

    for (int i = 0; i < list->size(); i++)
    {
        this->write_one_byte(list->at(i) & 0xFF);
        this->write_one_byte((list->at(i) >> 8) & 0xFF);
    }
}

void FileParser::read_audiofile_object(RPGAudioFile *audiofile)
{
    this->last_visited_function = "read_audiofile_object()";
    QVariantList list = read_object();
    if (list.at(0).toString() != "RPG::AudioFile")
        throw getException("RPG::AudioFile Expected");
    int num_params = list.at(1).toInt();

    for (int i = 0; i < num_params; i++)
    {
        QString current_symbol = this->read_symbol_or_link();
        if (current_symbol == "@pitch")
            audiofile->pitch = this->read_integer();
        else if (current_symbol == "@volume")
            audiofile->volume = this->read_integer();
        else if (current_symbol == "@name")
            audiofile->name = this->read_string();
    }


}

void FileParser::write_audiofile_object(RPGAudioFile *audiofile)
{
    this->last_visited_function = "write_audiofile_object()";

    this->write_object("RPG::AudioFile", 3);
    this->write_symbol_or_link("@volume");
    this->write_integer(audiofile->volume);

    this->write_symbol_or_link("@name");
    this->write_string(audiofile->name);

    this->write_symbol_or_link("@pitch");
    this->write_integer(audiofile->pitch);

}

void FileParser::read_tone(double *r, double *g, double *b, double *gray)
{
    this->read_one_byte(); // withdraw the u
    QString symbol = this->read_symbol_or_link();
    if (symbol == "Tone" || symbol == "Color") //old version had color instead of tone (feuergruen)
    {
        this->read_fixnum(); //32 unused

        this->file.read((char*)r, 8);
        this->file.read((char*)g, 8);
        this->file.read((char*)b, 8);
        this->file.read((char*)gray, 8);
    }
    else throw getException("Color tone expected");
}

void FileParser::write_tone(double r, double g, double b, double gray, bool color_tone = true)
{
    this->write_one_byte((int)'u');
    if (color_tone)
        this->write_symbol_or_link("Tone");
    else
        this->write_symbol_or_link("Color");

    this->write_fixnum(32);

    this->file.write((const char*)&r,8);
    this->file.write((const char*)&g,8);
    this->file.write((const char*)&b,8);
    this->file.write((const char*)&gray,8);

    this->increase_object_count("Tone");
}

void FileParser::read_move_route_object(RPGMoveRoute *move_route_object)
{
    this->last_visited_function = "read_move_route_object()";
    QVariantList params = this->read_object();
    if (params.at(0).toString() != "RPG::MoveRoute")
        throw getException("RPG::MoveRoute expected");
    int num_params = params.at(1).toInt();


    for (int i = 0; i < num_params; i++)
    {
         QString current_symbol = this->read_symbol_or_link();

         if (current_symbol == "@skippable")
             move_route_object->skippable = this->read_bool();
         else if (current_symbol == "@repeat")
             move_route_object->repeat = this->read_bool();
         else if (current_symbol == "@list")
         {
             int num_move_commands = this->read_array();
             for (int j = 0; j < num_move_commands; j++)
             {
                 //iterate over all move commands
                 QVariantList params = this->read_object();
                 if (params.at(0).toString() != "RPG::MoveCommand")
                     throw getException("RPG::MoveCommand expected");
                 RPGMoveCommand *mc = new RPGMoveCommand();
                 int num_params_for_move_command = params.at(1).toInt();
                 for (int k = 0; k < num_params_for_move_command; k++)
                 {
                     QString current_symbol = this->read_symbol_or_link();

                     if (current_symbol == "@code")
                        mc->code = this->read_integer();
                     else if (current_symbol == "@parameters")
                     {
                        int move_params_num = this->read_array();

                        for (int p = 0; p < move_params_num; p++)
                        {
                            // iterate over move command parameter list

                            //integer, and strings directly move to variant list
                            if (this->look_one_byte_ahead() == 'i' || this->look_one_byte_ahead() == '"' || this->look_one_byte_ahead() == 'I')
                                mc->parameters.append(this->read_variant()); //adjust for RPGStrings
                            else if (this->look_ahead_object_type() == "RPG::AudioFile")
                            {
                                this->read_audiofile_object(&mc->audiofile); //code 44 only
                            }
                            else
                                throw getException("Unknown move command parameter list type");

                        }

                     }
                 }
                 move_route_object->list.append(mc);

             }
         }

    }

}

void FileParser::write_move_route_object(RPGMoveRoute *move_route_object, QList<int> *references = 0)
{
    this->last_visited_function = "write_move_route_object()";

    this->write_object("RPG::MoveRoute", 3);

    this->write_symbol_or_link("@list");
    this->write_array(move_route_object->list.length());

    if (references != 0)
        references->append(this->object_count); //add to references list

    for (int i = 0; i < move_route_object->list.length(); i++)
    {
        this->write_object("RPG::MoveCommand", 2);

        this->write_symbol_or_link("@parameters");

        if (move_route_object->list.at(i)->code == 44) //Play SE
        {
            this->write_array(1);
            this->write_audiofile_object(&move_route_object->list.at(i)->audiofile);
            if (references != 0)
                references->append(this->object_count); //add to references list
        }
        else if (move_route_object->list.at(i)->code == 41) //Change Graphic
        {
            //we only do this manually because of the reference
            this->write_array(4);
            this->write_string(((RPGVariant)move_route_object->list.at(i)->parameters.at(0)).toString());
            if (references != 0)
                references->append(this->object_count); //add to references list
            this->write_integer(move_route_object->list.at(i)->parameters.at(1).toInt());
            this->write_integer(move_route_object->list.at(i)->parameters.at(2).toInt());
            this->write_integer(move_route_object->list.at(i)->parameters.at(3).toInt());

        }
        else if (move_route_object->list.at(i)->code == 45) //move route script
        {
            //we only do this manually because of the reference
            this->write_array(1);
            this->write_variant(move_route_object->list.at(i)->parameters.at(0));

            if (references != 0)
                references->append(this->object_count); //reference after variant
        }
        else
        {
            this->write_array(move_route_object->list.at(i)->parameters.length());
            if (references != 0)
                references->append(this->object_count); //add to references list
            for (int j = 0; j < move_route_object->list.at(i)->parameters.length(); j++)
            {
                this->write_variant(move_route_object->list.at(i)->parameters.at(j));
            }
        }


        this->write_symbol_or_link("@code");
        this->write_integer(move_route_object->list.at(i)->code);
    }

    this->write_symbol_or_link("@skippable");
    this->write_bool(move_route_object->skippable);

    this->write_symbol_or_link("@repeat");
    this->write_bool(move_route_object->repeat);
}

QByteArray FileParser::getHash()
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    this->file.open(QIODevice::ReadOnly);
    hash.addData(&this->file);
    this->file.close();
    return hash.result().toHex();
}



void FileParser::fixnum_test()
{
    this->file.setFileName("/tmp/test.dat");
    this->file.open(QIODevice::ReadWrite);

    for (int i = 0; i >= -513; i--)
        this->write_integer(i);

    this->file.seek(0);

    for (int i = 0; i >= -513; i--)
        qDebug() << this->read_integer();

    this->file.close();


}
