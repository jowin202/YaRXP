#include "animationeditor.h"
#include "ui_animationeditor.h"

AnimationEditor::AnimationEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnimationEditor)
{
    ui->setupUi(this);
}

AnimationEditor::~AnimationEditor()
{
    delete ui;
}
