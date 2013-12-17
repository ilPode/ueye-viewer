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

    /* True if the QImage is null */
    bool isImageNull() const;

    /* Return length between p1 and p2 (-1 if one is null) */
    double lineLength();

public slots:
    
    /* Toggle drawsShapes and shapeDetected variables */
    void onToggleViewShapes(bool toggle);
    void onShapesDetected();

    /* Clears the previous paintings */
    void clear();

    /* Zoom slots */
    void onZoomOriginal() { setZoom(1.0); };
    void onZoomOut();
    void onZoomIn();
    void setFitWindow(bool bFit);
    
    /* Draw line */
    void onToggleDrawLine(bool toggle); 

protected:
    void paintEvent(QPaintEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:
    void lineDrawn();

private:

    QImage image;
    double zoomFactor;
    bool fitWindow;

    QListWidget const * elList;
    bool drawShapes;
    bool shapesDetected;

    bool drawLine;
    QPointF p1, p2;
};

#endif
