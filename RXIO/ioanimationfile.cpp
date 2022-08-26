#include "ioanimationfile.h"

IOAnimationFile::IOAnimationFile()
{

}

IOAnimationFile::IOAnimationFile(QString path, QList<RPGAnimation *> *animation_list)
{
    this->last_visited_function = "IOAnimationFile";
    this->symbol_cache.clear();

    this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::ReadOnly);

    check_header();
    int length = read_array();
    this->read_one_byte(); //withdraw first zero

    for (int i = 1 ; i < length; i++)
    {
        RPGAnimation *current_animation = new RPGAnimation;

        QVariantList list = read_object();
        if (list.at(0).toString() != "RPG::Animation")
            throw getException("RPG::Animation expected");
        for (int j = 0; j < list.at(1).toInt(); j++)
        {
            QString current_symbol = read_symbol_or_link();
            current_animation->param_order << current_symbol;



            if (current_symbol == "@name")
                current_animation->name = this->read_string();
            else if (current_symbol == "@animation_name")
                current_animation->animation_name = this->read_string();
            else if (current_symbol == "@id")
                current_animation->id = this->read_integer();
            else if (current_symbol == "@position")
                current_animation->position = this->read_integer();
            else if (current_symbol == "@animation_hue")
                current_animation->animation_hue = this->read_integer();
            else if (current_symbol == "@frame_max")
                current_animation->frame_max = this->read_integer();
            else if (current_symbol == "@frames")
            {
                int num_frames = this->read_array();
                for (int j = 0; j < num_frames; j++)
                {
                    RPGAnimationFrame *current_frame = new RPGAnimationFrame;
                    QVariantList list = read_object();
                    if (list.at(0).toString() != "RPG::Animation::Frame")
                        throw getException("RPG::Animation::Frame expected");

                    for (int k = 0; k < list.at(1).toInt(); k++)
                    {
                        QString current_symbol = read_symbol_or_link();
                        current_frame->param_order << current_symbol;

                        if (current_symbol == "@cell_max")
                            current_frame->cell_max = this->read_integer();
                        if (current_symbol == "@cell_data")
                            this->read_table(&current_frame->cell_data);
                    }
                    current_animation->frames.append(current_frame);
                }
            }
            else if (current_symbol == "@timings")
            {
                int num_timings = this->read_array();
                for (int j = 0; j < num_timings; j++)
                {
                    RPGAnimationTiming *current_timing = new RPGAnimationTiming;
                    QVariantList list = read_object();
                    if (list.at(0).toString() != "RPG::Animation::Timing")
                        throw getException("RPG::Animation::Timing expected");

                    for (int k = 0; k < list.at(1).toInt(); k++)
                    {
                        QString current_symbol = read_symbol_or_link();
                        current_timing->param_order << current_symbol;

                        if (current_symbol == "@condition")
                            current_timing->condition = this->read_integer();
                        else if (current_symbol == "@frame")
                            current_timing->frame = this->read_integer();
                        else if (current_symbol == "@flash_duration")
                            current_timing->flash_duration = this->read_integer();
                        else if (current_symbol == "@flash_scope")
                            current_timing->flash_scope = this->read_integer();
                        else if (current_symbol == "@flash_color")
                            this->read_tone(&current_timing->r,&current_timing->g,
                                            &current_timing->b,&current_timing->alpha);
                        else if (current_symbol == "@se")
                            this->read_audiofile_object(&current_timing->se);
                    }
                    current_animation->timings.append(current_timing);
                }
            }
        }
        animation_list->append(current_animation);
    }

    this->file.close();
}

void IOAnimationFile::write_to_file(QString path, QList<RPGAnimation *> *animation_list)
{
    this->last_visited_function = "write_to_file";
    this->symbol_cache.clear();

    if (!path.isEmpty())
        this->path = path;
    this->file.setFileName(path);
    this->file.open(QIODevice::WriteOnly);
    this->write_header();

    this->write_array(animation_list->length()+1); //zero at beginning
    this->write_one_byte((int)'0');

    for (int i = 0; i < animation_list->length(); i++)
    {
        RPGAnimation *current_animation = animation_list->at(i);
        this->write_object("RPG::Animation", 8);
        for (int j = 0; j < current_animation->param_order.length(); j++)
        {
            QString current_symbol = current_animation->param_order.at(j);
            this->write_symbol_or_link(current_symbol);

            if (current_symbol == "@id")
                this->write_integer(current_animation->id);
            else if (current_symbol == "@position")
                this->write_integer(current_animation->position);
            else if (current_symbol == "@animation_hue")
                this->write_integer(current_animation->animation_hue);
            else if (current_symbol == "@frame_max")
                this->write_integer(current_animation->frame_max);
            else if (current_symbol == "@name")
                this->write_string(current_animation->name);
            else if (current_symbol == "@animation_name")
                this->write_string(current_animation->animation_name);
            else if (current_symbol == "@frames")
            {
                this->write_array(current_animation->frames.length());
                for (int k = 0; k < current_animation->frames.length(); k++)
                {
                    this->write_object("RPG::Animation::Frame", 2);
                    RPGAnimationFrame *current_frame = current_animation->frames.at(k);

                    for (int l = 0; l < current_frame->param_order.length(); l++)
                    {
                        QString current_symbol = current_animation->frames.at(k)->param_order.at(l);
                        this->write_symbol_or_link(current_symbol);

                        if (current_symbol == "@cell_max")
                            this->write_integer(current_frame->cell_max);
                        else if (current_symbol == "@cell_data")
                            this->write_table(&current_frame->cell_data);
                    }
                }
            }
            else if (current_symbol == "@timings")
            {
                this->write_array(current_animation->timings.length());
                for (int k = 0; k < current_animation->timings.length(); k++)
                {
                    this->write_object("RPG::Animation::Timing", 6);
                    RPGAnimationTiming *current_timing = current_animation->timings.at(k);

                    for (int l = 0; l < current_timing->param_order.length(); l++)
                    {
                        QString current_symbol = current_animation->timings.at(k)->param_order.at(l);
                        this->write_symbol_or_link(current_symbol);

                        if (current_symbol == "@frame")
                            this->write_integer(current_timing->frame);
                        else if (current_symbol == "@flash_duration")
                            this->write_integer(current_timing->flash_duration);
                        else if (current_symbol == "@flash_scope")
                            this->write_integer(current_timing->flash_scope);
                        else if (current_symbol == "@condition")
                            this->write_integer(current_timing->condition);
                        else if (current_symbol == "@se")
                            this->write_audiofile_object(&current_timing->se);
                        else if (current_symbol == "@flash_color")
                            this->write_tone(current_timing->r, current_timing->g,
                                             current_timing->b, current_timing->alpha, false);
                    }
                }
            }

        }
    }

    this->file.close();
}
