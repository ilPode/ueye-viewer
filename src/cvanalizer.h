/*
 * file    cvanalizer.h
 * version 0.1
 *
 * Image class with openCV capabilities
 *
 */
#ifndef CVANALIZER_H
#define CVANALIZER_H

#include <opencv2/opencv.hpp>
#include <QImage>
#include <QRect>
#include <QtDebug>

#define DEFAULT_MAX_WIDTH 800
#define DEFAULT_MAX_HEIGHT 800

/* Enum of shape types for detection */

enum eShape {
    SHAPE_NONE = 0,
    SHAPE_CIRCLE = 1,
    SHAPE_ELLIPSE = 2
};

/* Structure of options for shape detection */

struct sDetectionOptions {
    eShape shape;
    int minRad;
    int maxRad;
    int minDist;

    int accumulatorDim;
    int cannyTh;
    int accumulatorTh;
};

const sDetectionOptions defaultOptions = {
    SHAPE_NONE,
    0,
    100,
    5,
    1,
    100,
    300
};

class CvAnalizer : public QObject {
    Q_OBJECT
    
    public:
        CvAnalizer(QObject * parent = 0); 
        CvAnalizer(QImage const * image, QObject * parent = 0);
        ~CvAnalizer();
        
        bool setImage(QImage const * image, int mWidth = DEFAULT_MAX_WIDTH, int mHeight = DEFAULT_MAX_HEIGHT);
        void setROI(QRect const * roi);
        void findShapes(sDetectionOptions opts = defaultOptions); 
        
        int getCirclesNum() const;
        void getCircle(int i, float * circle) const;
        float const * getScaledCircle(int i) const;
        int getWidth() const;
        int getHeight() const;
        
    protected:
        bool convertToGrayscale(QImage const * source, IplImage * pDestImage) const;
        //>bool Scale(QImage const * source, IplImage * pDestImage) const;
        int getIPLDepth(QImage::Format format) const; 
        int getChannelsNum(QImage::Format format) const;
    
    private:
        IplImage * cv_image;
        CvMemStorage * cv_memCircles;
        CvSeq * cv_seqCircles;
        double scalingFactor;

        //CvMemStorage * cv_memEllipses;
        //CvSeq * cv_seqEllipses;
        //CvMemStorage * cv_memContours;
        //CvSeq * cv_seqContours;
};


#endif
