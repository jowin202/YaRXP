#ifndef ANIMATIONEDITOR_H
#define ANIMATIONEDITOR_H

#include <QWidget>

namespace Ui {
class AnimationEditor;
}

class AnimationEditor : public QWidget
{
    Q_OBJECT

public:
    explicit AnimationEditor(QWidget *parent = nullptr);
    ~AnimationEditor();

private:
    Ui::AnimationEditor *ui;
};

#endif // ANIMATIONEDITOR_H
