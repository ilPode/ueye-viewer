/*
 * file    paintlabel.h
 * version 0.1
 *
 * label to paint images
 *
 */

#ifndef PAINTLABEL_H
#define PAINTLABEL_H

#include "detection.h"

#include <QImage>
#include <QLabel>
#include <QListWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

#define SHAPE_PEN_WIDTH 3
#define ZOOM_STEP 0.1
#define ZOOM_MIN 0.1
#define ZOOM_MAX 2.0

class PaintLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PaintLabel(QWidget *parent = 0);

    /* Draws an Image on the label */
    void setImage(QImage &image);

    /* Draws an pixmap on the label */
    void setPixmap(const QPixmap &pixmap);

    /* Adjusts the size of the widget to fit its contents.*/
    void adjustSize();

    /* Set and retrieve the zoom factor */
    double getZoom() const;
    void setZoom(double zoom); 

    /* Set the source for drawing circles and ellipses */
    void setEllipsesList(QListWidget const * list);

    /* Get pointer to QImage */
    QImage const * getImage() const;

    /* True if the QImage is not null */
    bool isImagePresent() const;

public slots:
    
    /* Toggle draws shapes on the image */
    void onToggleViewShapes(bool toggle);
    
    /* Clears the previous paintings */
    void clear();

    /* Zoom slots */
    void onZoomOriginal() { setZoom(1.0); };
    void onZoomOut();
    void onZoomIn();
    void setFitWindow(bool bFit);

protected:
    void paintEvent(QPaintEvent *ev);

private:

    QImage image;
    double zoomFactor;
    bool fitWindow;

    QListWidget const * elList;
    bool drawShapes;
};

#endif
