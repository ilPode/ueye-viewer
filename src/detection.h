/*
 * file    detection.h
 * version 0. version 0.11
 *
 * DetectorThread and EllipsesModel class header
 *
 */

#ifndef DETECTION_H
#define DETECTION_H

#include <QImage>
#include <QRect>
#include <QtDebug>
#include <QThread>
#include <QMutex>

#include "cvanalizer.h"

#define INTERVAL_DEFAULT 10

/*
 * Thread for shape detection
 */

class DetectionThread : public QThread {
    Q_OBJECT

    public:
        DetectionThread( QObject * parent = 0);
        DetectionThread(QImage const * image, QObject * parent = 0);
        ~DetectionThread();

        void run();

        CvAnalizer const * getCVImage() const;
    
        QMutex * cvMutex;
        
    public slots:
        void setImage(QImage const * image);
        void setInterval(int interval);
        void setShape(int shape);
        void setMinimumRadius(int rad);
        void setMaximumRadius (int m);
        void setCannyThreshold (int th);
        void setAccumulatorThreshold (int th);
        void setMinimumDistance (int d);
        void setAccumulatorFactor (int f);
        void toggleGaussianBlur (bool toggle);
        void setGaussianWidth (int s);
        void runDetection(bool run);

    signals:
        void detectionFinished(CvAnalizer const * cva);
        void shapesDetected();
        
    private:
        CvAnalizer * cvImg;
        int sleepInterval;
        bool runDetect;
        eShape detectionDone;
        sDetectionOptions options;
};

#endif
