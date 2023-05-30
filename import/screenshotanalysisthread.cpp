#include "screenshotanalysisthread.h"

ScreenshotAnalysisThread::ScreenshotAnalysisThread(QImage screenshot, qreal threshold)
{
    this->screenshot = screenshot;
    this->threshold = threshold;
}

void ScreenshotAnalysisThread::run()
{
    QMap< QPair<int,int>, int > result;

    QImage current_tile_img;
    int t = 0;
    for (int y = 0; 32*(y+1) <= screenshot.height(); y++)
    {
        for (int x = 0; 32*(x+1) <= screenshot.width(); x++)
        {
            qDebug() << x << y;
            QPair<int,int> current_pair(x,y);
            if (result.contains(current_pair)) continue;
            else
            {
                result.insert(current_pair,t);
                current_tile_img = screenshot.copy(32*x,32*y,32,32);
                for (int y2 = 0; 32*(y2+1) <= screenshot.height(); y2++)
                {
                    for (int x2 = 0; 32*(x2+1) <= screenshot.width(); x2++)
                    {
                        if (x==x2 && y==y2) continue;
                        QPair<int,int> current_pair2(x2,y2);
                        if (result.contains(current_pair2)) continue;

                        qint64 diff = 0;
                        for (int yp = 0; yp < 32; yp++)
                        {
                            for (int xp = 0; xp < 32; xp++)
                            {
                                QColor col1 = current_tile_img.pixelColor(xp,yp);
                                QColor col2 = screenshot.pixelColor(32*x2+xp,32*y2+yp);
                                diff += qAbs(col1.red()-col2.red())+qAbs(col1.green()-col2.green())+qAbs(col1.blue()-col2.blue());
                            }
                        }

                        if (diff < threshold*1024*255*3)
                        {
                            result.insert(current_pair2, t);
                        }
                    }
                }

                t++;
            }
        }
    }


    emit finished(result);
}
