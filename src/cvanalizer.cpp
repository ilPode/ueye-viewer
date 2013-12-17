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
    QImage * pImage;
    if (newImage->width() > mWidth || newImage->height() > mHeight) {
        pImage = new QImage(newImage->scaled(mWidth, mHeight, Qt::KeepAspectRatio));
    }
    else {
        pImage = new QImage(*newImage);
    }

    ret = convertToGrayscale(pImage, &cv_image);
    if (ret) {
        scalingFactor = (double) cv_image->width / (double) newImage->width();
    }
    delete pImage;
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
        IplImage *tmpImg = cvCloneImage(cv_image);
        if (opts.gaussianBlur) {
            cvSmooth(tmpImg, tmpImg, CV_GAUSSIAN, opts.blurWidth, opts.blurWidth );
        }
        //cvSaveImage("image_smooth.png", cv_image); 
        //cvThreshold(cv_image, cv_image, THRESHOLD_VALUE, THRESHOLD_MAX, CV_THRESH_BINARY_INV);
        cv_seqCircles = cvHoughCircles(
                tmpImg,
                cv_memCircles, 
                CV_HOUGH_GRADIENT,
                opts.accumulatorDim,    // Accumulator resolution ratio
                opts.minDist,           // Min distance between two centers
                opts.cannyTh,           // Canny edge threshold
                opts.accumulatorTh,     // Accumulator threshold
                (int) (opts.minRad * MIN(cv_image->width, cv_image->height)), // Min radius
                (int) (opts.maxRad * MIN(cv_image->width, cv_image->height)) //Max radius
                );
        cvReleaseImage(&tmpImg);
        //cvSaveImage("image_final.png", cv_image); 
    }
}

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

bool CvAnalizer :: convertToGrayscale (QImage const * source, IplImage ** ppDestImage) const {

    bool ret = true;
    if (! source->isNull()) {

        int depth;
        int chNum;

        IplImage * destImage = cvCreateImage(cvSize(source->width(), source->height()), IPL_DEPTH_8U, 1);
        IplImage * srcImage;

        QImage::Format format = source->format();
        switch(format) {
            case QImage::Format_Indexed8:
                if (source->isGrayscale()) {
                    depth = getIPLDepth(source->format());
                    chNum = getChannelsNum(source->format());
                    srcImage = cvCreateImageHeader(cvSize(source->width(), source->height()), depth, chNum);

                    cvSetData(srcImage, (void *)source->bits(), source->bytesPerLine());
                    cvCopy(srcImage, destImage);
                }
                else {
                    QImage rgbSource = source->convertToFormat(QImage::Format_RGB888);
                    depth = getIPLDepth(rgbSource.format());
                    chNum = getChannelsNum(rgbSource.format());
                    srcImage = cvCreateImageHeader(cvSize(rgbSource.width(), rgbSource.height()), depth, chNum);

                    cvSetData(srcImage, (void *)rgbSource.bits(), rgbSource.bytesPerLine());
                    cvCvtColor(srcImage, destImage, CV_RGB2GRAY);
                }
                break;
            
            case QImage::Format_RGB32:
            case QImage::Format_ARGB32: 
            case QImage::Format_RGB888:
                depth = getIPLDepth(source->format());
                chNum = getChannelsNum(source->format());
                srcImage = cvCreateImageHeader(cvSize(source->width(), source->height()), depth, chNum);
                cvSetData(srcImage, (void *)source->bits(), source->bytesPerLine());
                cvCvtColor(srcImage, destImage, CV_RGB2GRAY);
                break;

           /* This is not needed because cvCvtColor takes care of the additional channel
            case QImage::Format_RGB32:
            case QImage::Format_ARGB32: {
                qDebug() << "Format: RGB32 o ARGB32";
                IplImage * tempImage = cvCreateImage(cvSize(source->width(), source->height()), IPL_DEPTH_8U, 3);
                cvCvtColor(srcImage, tempImage, CV_RGBA2RGB);
                cvCvtColor(tempImage, pDestImage, CV_RGB2GRAY);
                cvReleaseImage(&tempImage);
                break;
            }*/
            case QImage::Format_RGB16: {
                IplImage * tempImage = cvCreateImage(cvSize(source->width(), source->height()), IPL_DEPTH_16U, 1);
                cvSetData(srcImage, (void *)source->bits(), source->bytesPerLine());
                cvCvtColor(srcImage, tempImage, CV_BGR5652GRAY);
                cvConvertScale(tempImage, destImage);
                cvReleaseImage(&tempImage);
                break;
            }

            default:
                qWarning() << "Image format not supported: " << format << "(in function: CvAnalizer::convertToGrayscale)";
                cvReleaseImageHeader(&destImage);
                ret = false;
            }

        cvReleaseImageHeader(&srcImage);
        if (ret) {
            *ppDestImage = destImage;
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

