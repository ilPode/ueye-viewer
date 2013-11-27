/*
 * file    cvanalizer.cpp
 * version 0.1
 *
 * Image class with openCV capabilities
 *
 */

#include "cvanalizer.h"

/* Constructors and destructors */

CvAnalizer :: CvAnalizer (QObject * parent) : QObject(parent) {
    
    cv_image = NULL;
    cv_memCircles = cvCreateMemStorage();
    cv_seqCircles = NULL;
    scalingFactor = 0;
    //cv_memEllipses = cvCreateMemStorage();
    //cv_seqEllipses = NULL;
}

CvAnalizer :: CvAnalizer(QImage const * image, QObject * parent) : QObject(parent) {
    
    cv_image = NULL; 
    if (!setImage(image)) {
        
        qDebug() << "Cannot set image (in function: CvAnalizer constructor)";
        scalingFactor = 0;
    }

    cv_memCircles = cvCreateMemStorage();
    cv_seqCircles = NULL;
    //cv_memEllipses = cvCreateMemStorage();
    //cv_seqEllipses = NULL;
}

CvAnalizer :: ~CvAnalizer() {
    
    delete cv_seqCircles;
    cvReleaseMemStorage(&cv_memCircles);
    //delete cv_seqEllipses;
    //cvReleaseMemStorage(&cv_memEllipses);
    //> RELEASE CV IMAGE (non me ricordo la funzione)
}

/* Public methods */

bool CvAnalizer :: setImage (QImage const * newImage, int mWidth, int mHeight) {
    bool ret;
    QImage::Format format = newImage->format();
    if (format == QImage::Format_Indexed8 ||
            format == QImage::Format_RGB888 ||
            format == QImage::Format_RGB32 ||
            format == QImage::Format_ARGB32 ||
            format == QImage::Format_RGB16 ) {
        
        if (newImage->width() > mWidth || newImage->height() > mHeight) {
            QImage image = newImage->scaled(mWidth, mHeight, Qt::KeepAspectRatio);
            cv_image = cvCreateImage(cvSize(image.width(), image.height()), IPL_DEPTH_8U, 1);
            ret = convertToGrayscale(&image, cv_image);
        }
        else {
            cv_image = cvCreateImage(cvSize(newImage->width(), newImage->height()), IPL_DEPTH_8U, 1);
            ret = convertToGrayscale(newImage, cv_image);
        }
        scalingFactor = (double) cv_image->width / (double) newImage->width();
    }
    else {

        qWarning() << "Image format not supported: " << format << " (in function: CvAnalizer::setImage)";
        ret = false;
    }
    return ret;
}


void CvAnalizer :: setROI (QRect const * roi) {
    if (roi) {
        int x, y, w, h;
        roi->getRect(&x, &y, &w, &h);
        CvRect rect = cvRect(x, y, w, h);
        cvSetImageROI(cv_image, rect);
    }
}

void CvAnalizer :: findShapes (sDetectionOptions opts) {

    if ((opts.shape & SHAPE_CIRCLE) != 0) {
        //cvClearMemStorage(cv_memCircles);
        cvSaveImage("image_original.png", cv_image); 
        cvSmooth(cv_image, cv_image, CV_GAUSSIAN, 5, 5 );
        cvSaveImage("image_smooth.png", cv_image); 
        cvThreshold(cv_image, cv_image, THRESHOLD_VALUE, THRESHOLD_MAX, CV_THRESH_BINARY_INV);
        cvSaveImage("image_thresh.png", cv_image); 
        cv_seqCircles = cvHoughCircles(
                cv_image,
                cv_memCircles, 
                CV_HOUGH_GRADIENT,
                opts.accumulatorDim,    // Accumulator resolution ratio
                opts.minDist,           // Min distance between two centers
                opts.cannyTh,           // Canny edge threshold
                opts.accumulatorTh,     // Accumulator threshold
                opts.minRad * (cv_image->width > cv_image->height ? cv_image->height : cv_image->width), // Min radius
                opts.maxRad * (cv_image->width > cv_image->height ? cv_image->height : cv_image->width) // Max radius
                );
        for( int i = 0; i < cv_seqCircles->total; i++ ) {
            float* p = (float*) cvGetSeqElem(cv_seqCircles, i);
            CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));
            cvCircle(cv_image, pt, cvRound(p[2]), CV_RGB(0x88,0x88,0x88));
        }
        cvSaveImage("image_final.png", cv_image); 
    }
}

/*
void Image :: findEllipses(...) {
}*/


int CvAnalizer :: getCirclesNum() const {
    if (cv_seqCircles) 
        return cv_seqCircles->total;
    else
        return 0;
}

void CvAnalizer :: getCircle( int i, float * circle ) const {
    float const * sCircle = getScaledCircle(i);
    if (sCircle != NULL) {
        circle[0] = sCircle[0] / scalingFactor;
        circle[1] = sCircle[1] / scalingFactor;
        circle[2] = sCircle[2] / scalingFactor;
    }
}

float const * CvAnalizer :: getScaledCircle( int i ) const {
    if (cv_seqCircles)
        return (float *) cvGetSeqElem(cv_seqCircles, i);
    else
        return NULL;
}

int CvAnalizer :: getWidth() const {
    return cv_image->width;
}

int CvAnalizer :: getHeight() const {
    return cv_image->height;
}

/* Protected Methods */

bool CvAnalizer :: convertToGrayscale (QImage const * source, IplImage * pDestImage) const {

    bool ret = true;
    if (! source->isNull()) {

        int depth = getIPLDepth(source->format());
        int chNum = getChannelsNum(source->format());

        if (depth !=0 && chNum != 0) {
            IplImage * srcImage = cvCreateImageHeader(cvSize(source->width(), source->height()), depth, chNum);
            cvSetData(srcImage, (void *)source->bits(), source->bytesPerLine());
            
            QImage::Format format = source->format();
            switch(format) {
                case QImage::Format_Indexed8:
                    cvCopy(srcImage, pDestImage);
                    break;
                
                case QImage::Format_RGB888:
                    cvCvtColor(srcImage, pDestImage, CV_RGB2GRAY);
                    break;
                
                case QImage::Format_RGB32:
                case QImage::Format_ARGB32: {
                    IplImage * tempImage = cvCreateImage(cvSize(source->width(), source->height()), IPL_DEPTH_8U, 3);
                    cvCvtColor(srcImage, tempImage, CV_RGBA2RGB);
                    cvCvtColor(tempImage, pDestImage, CV_RGB2GRAY);
                    cvReleaseImage(&tempImage);
                    break;
                }
                case QImage::Format_RGB16: {
                    IplImage * tempImage = cvCreateImage(cvSize(source->width(), source->height()), IPL_DEPTH_16U, 1);
                    cvCvtColor(srcImage, tempImage, CV_BGR5652GRAY);
                    cvConvertScale(tempImage, pDestImage);
                    cvReleaseImage(&tempImage);
                    break;
                }
                default:
                    qWarning() << "Image format not supported: " << format << "(in function: CvAnalizer::convertToGrayscale)";
                    ret = false;
                }

            cvReleaseImageHeader(&srcImage);
        }

        else {
            qDebug() << "Invalid depth or channels number (in function CvAnalizer::convertToGrayscale)";
            ret = false;
        }
    }
    else {
        qDebug() << "Null image (in function CvAnalizer::convertToGrayscale)";
        ret = false;
    }
    return ret;
}

int CvAnalizer :: getIPLDepth (QImage::Format format) const {

    switch(format) {
        case QImage::Format_Indexed8:
        case QImage::Format_RGB888:
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
            return IPL_DEPTH_8U;
        case QImage::Format_RGB16: // RGB(5-6-5) -> 16bit and 1 channel?
            return IPL_DEPTH_16U;
        default:
            qWarning() << "Image format not supported:" << format << " (in function: CvAnalizer::getIPLDepth)";
            return 0;
   }
}

int CvAnalizer :: getChannelsNum (QImage::Format format) const {

    switch(format) {
        case QImage::Format_Indexed8:
        case QImage::Format_RGB16: // RGB(5-6-5) -> 16bit and 1 channel?
            return 1;
        case QImage::Format_RGB888:
            return 3;
        case QImage::Format_RGB32:
        case QImage::Format_ARGB32:
            return 4;
        default:
            qWarning() << "Image format not supported:" << format << " (in function: CvAnalizer::getChannelsNum)";
            return 0;
    }
}

/* Not supported QImage formats:
case QImage::Format_Mono:
case QImage::Format_MonoLSB:
case QImage::Format_ARGB32_Premultiplied:
case QImage::Format_ARGB8565_Premultiplied:
case QImage::Format_RGB666:
case QImage::Format_ARGB6666_Premultiplied:
case QImage::Format_RGB555:
case QImage::Format_ARGB8555_Premultiplied:
case QImage::Format_RGB444:
case QImage::Format_ARGB4444_Premultiplied:
*/

