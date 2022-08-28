#include "RXObjects/rpgsystem.h"
#include "ioscriptfile.h"


IOScriptFile::IOScriptFile()
{

}

void IOScriptFile::read_file_to_script_folder(RPGSystem *system)
{
    this->last_visited_function = "read_file_to_script_folder";
    this->symbol_cache.clear();

    this->path = system->data_dir + "Scripts.rxdata";
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();

    QDir dir(system->data_dir + "Scripts");
    if (dir.exists())
        dir.removeRecursively();
    dir.mkdir(system->data_dir + "Scripts");

    int num_scripts = this->read_array();
    for (int i = 0; i < num_scripts; i++)
    {
        if (this->read_array() != 3)
            throw getException("Script should have 3 parameters!");
        int uncompressed_size = this->read_integer();

        RPGString name = this->read_string();

        if (this->read_one_byte() != '"')
            throw getException("String expected");

        int size = this->read_fixnum();

        QFile script_file(system->data_dir + "Scripts" + QDir::separator() + name + ".rb");
        script_file.open(QIODevice::WriteOnly);

        QByteArray buffer = this->file.read(size);
        buffer = buffer.prepend(uncompressed_size&0xFF);
        uncompressed_size >>= 8;
        buffer = buffer.prepend(uncompressed_size&0xFF);
        uncompressed_size >>= 8;
        buffer = buffer.prepend(uncompressed_size&0xFF);
        uncompressed_size >>= 8;
        buffer = buffer.prepend(uncompressed_size&0xFF);

        script_file.write(qUncompress(buffer));

        script_file.close();
    }
    this->file.close();
}

QStringList IOScriptFile::read_file_names_from_scripts(RPGSystem *system)
{
    this->last_visited_function = "read_file_names_from_scripts";
    this->symbol_cache.clear();

    this->path = system->data_dir + "Scripts.rxdata";
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    QStringList result;


    int num_scripts = this->read_array();
    for (int i = 0; i < num_scripts; i++)
    {
        if (this->read_array() != 3)
            throw getException("Script should have 3 parameters!");

        this->read_integer(); //don't need this here

        RPGString name = this->read_string();
        result.append(name);

        if (this->read_one_byte() != '"')
            throw getException("String expected");

        int size = this->read_fixnum();
        this->file.seek(this->file.pos() + size);
    }

    this->file.close();
    return result;
}

void IOScriptFile::write_script_file(RPGSystem *system, QStringList list)
{
    this->last_visited_function = "write_script_file";
    this->symbol_cache.clear();

    /*
    this->path = system->data_dir + "Scripts1.rxdata";
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    check_header();
    */



    int num_scripts = list.length();
    //this->write_array(num_scripts);
    for (int i = 0; i < num_scripts; i++)
    {
        //this->write_array(3);


        QFile script_file(system->data_dir + "Scripts" + QDir::separator() + list.at(i) + ".rb");
        script_file.open(QIODevice::ReadOnly);
        QByteArray buffer = qCompress(script_file.readAll());
        qDebug() << buffer.toHex();

        return;

        /*
        int uncompressed_size = this->read_integer();

        RPGString name = this->read_string();

        if (this->read_one_byte() != '"')
            throw getException("String expected");

        int size = this->read_fixnum();

        script_file.open(QIODevice::WriteOnly);

        buffer = buffer.prepend(uncompressed_size&0xFF);
        uncompressed_size >>= 8;
        buffer = buffer.prepend(uncompressed_size&0xFF);
        uncompressed_size >>= 8;
        buffer = buffer.prepend(uncompressed_size&0xFF);
        uncompressed_size >>= 8;
        buffer = buffer.prepend(uncompressed_size&0xFF);

        script_file.write(qUncompress(buffer));


        script_file.close();
        */
    }
    //this->file.close();
}
