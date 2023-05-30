#ifndef SCREENSHOTANALYSISTHREAD_H
#define SCREENSHOTANALYSISTHREAD_H

#include <QThread>
#include <QMap>
#include <QImage>
#include <QPoint>
#include <QDebug>

class ScreenshotAnalysisThread : public QThread
{
    Q_OBJECT
public:
    ScreenshotAnalysisThread(QImage screenshot, qreal threshold);
    void run();


signals:
    void finished(QMap<QPair<int,int>, int > result);


private:
    QImage screenshot;
    qreal threshold;
};

#endif // SCREENSHOTANALYSISTHREAD_H
