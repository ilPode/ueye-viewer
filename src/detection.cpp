/*
 * file    detection.cpp
 * version 0.1
 *
 * DetectorThread and EllipsesModel class implementation
 *
 */

#include "detection.h"

/* 
 * DetectorThread implementation
 */

/* Constuctors and destructor */

DetectionThread :: DetectionThread (QObject * parent) 
    : QThread (parent) {
    
    cvImg = new CvAnalizer(this);
    cvMutex = new QMutex;

    options = defaultOptions;
    runDetect = false;
    sleepInterval = INTERVAL_DEFAULT;
    detectionDone = SHAPE_NONE;
}

DetectionThread :: DetectionThread (QImage const * image, QObject * parent) 
    : QThread (parent) {
    
    cvImg = new CvAnalizer(image, this);
    cvMutex = new QMutex;

    options = defaultOptions;
    runDetect = false;
    sleepInterval = INTERVAL_DEFAULT;
    detectionDone = SHAPE_NONE;
}

DetectionThread :: ~DetectionThread () {
    delete cvImg;
    delete cvMutex;
}

/* Public methods */

void DetectionThread :: setImage (QImage const * image) {
    cvMutex->lock();
    cvImg->setImage(image);
    cvMutex->unlock();
    detectionDone = SHAPE_NONE;
}

void DetectionThread :: setInterval (int interval) {
    if (interval > 0) {
        sleepInterval = interval;
    }
}

void DetectionThread :: setShape (int shape) {
    
    eShape s = (eShape) shape;
    if ( (shape < 0) || (shape > 2) ) {
        options.shape = SHAPE_NONE;
    }
    else {
        options.shape = s;
    }
}

void DetectionThread :: setMinimumRadius (int m) {
    
    if (m <= 0 && m < 1) 
        options.minRad = m;
    detectionDone = SHAPE_NONE;
}

void DetectionThread :: setMaximumRadius (int m) {
    
    if (m > 0 && m <= 1) 
        options.minRad = m;
    detectionDone = SHAPE_NONE;
}

void DetectionThread :: setCannyThreshold (int th) {
    
    if (th > 0)
        options.cannyTh = th;
    detectionDone = SHAPE_NONE;
}

void DetectionThread :: setAccumulatorThreshold (int th) {
    
    if (th > 0)
        options.accumulatorTh = th;
    detectionDone = SHAPE_NONE;
}

void DetectionThread :: setMinimumDistance (int d) {
    
    if (d > 0)
        options.minDist = d;
    detectionDone = SHAPE_NONE;
}

void DetectionThread :: setAccumulatorFactor (int f) {
    
    if (f >=1)
        options.accumulatorDim = f;
    detectionDone = SHAPE_NONE;
}

void DetectionThread :: runDetection (bool run) {
    runDetect = run;
    if (!this->isRunning()) {
        this->start();
    }
}

void DetectionThread :: run () {
    while (runDetect) {
        if ((detectionDone ^ options.shape) != 0) {
            cvMutex->lock();
            cvImg->findShapes(options);
            cvMutex->unlock();
            detectionDone = (eShape) (detectionDone | options.shape);
            emit detectionFinished(cvImg);
        }
        msleep(sleepInterval);
    }
}

