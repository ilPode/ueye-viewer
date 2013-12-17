/*
 * file    paintlabel.cpp
 * version 0.1
 *
 * label to paint images
 *
 */

#include "paintlabel.h"

PaintLabel::PaintLabel(QWidget *parent) : QLabel(parent) {
    zoomFactor = 1.0;
    fitWindow = false;
    shapesDetected = false;
    drawShapes = true;
    drawLine = false;
    p1 = QPointF();
    p2 = QPointF();
}

void PaintLabel::setPixmap(const QPixmap &pixmap)
{
    // we also can draw a pixmap
    // conversion is slow
    image = pixmap.toImage();
    shapesDetected = false;

    adjustSize();
}

void PaintLabel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);

    if (!image.isNull())
    {

        int width = this->width();
        int height = this->height();
        qreal posX = 0.0; 
        qreal posY = 0.0;

        QRectF target(posX, posY, width, height);

        // draw the image
        QPainter painter(this);
        painter.drawImage(target, image);
        double hFactor = (double) height / (double) image.height();
        double wFactor = (double) width / (double) image.width();

        // draw line
        if (drawLine) {
            QPen pen(Qt::cyan);
            pen.setWidth(2);
            painter.setPen(pen);

            if (!p1.isNull()) {
                painter.drawLine(
                        p1.x() * wFactor - 5, 
                        p1.y() * hFactor, 
                        p1.x() * wFactor + 5, 
                        p1.y() * hFactor 
                        );
                painter.drawLine(
                        p1.x() * wFactor,
                        p1.y() * hFactor - 5,
                        p1.x() * wFactor,
                        p1.y() * hFactor + 5
                        );
            }
            if (!p2.isNull()) {
                painter.drawLine(
                        p2.x() * wFactor - 5, 
                        p2.y() * hFactor, 
                        p2.x() * wFactor + 5, 
                        p2.y() * hFactor 
                        );
                painter.drawLine(
                        p2.x() * wFactor,
                        p2.y() * hFactor - 5,
                        p2.x() * wFactor,
                        p2.y() * hFactor + 5
                        );
            }
            if (!p1.isNull() && !p2.isNull()) {
                painter.drawLine(
                        p1.x() * wFactor,
                        p1.y() * hFactor,
                        p2.x() * wFactor,
                        p2.y() * hFactor
                        );   
            }
        }
        
        // draw detected shapes
        if (drawShapes && shapesDetected) {
            painter.setBrush(Qt::NoBrush);
            QListWidgetItem * ellipse;
            
            for (int i = 0; i < elList->count(); i++) {
                ellipse = elList->item(i);
                QRectF elRect = ellipse->data(Qt::UserRole + 1).toRectF();
                elRect.setRect(
                        elRect.x() * wFactor,
                        elRect.y() * hFactor,
                        elRect.width() * wFactor,
                        elRect.height() * hFactor
                        );
                
                QPen pen(ellipse->data(Qt::DecorationRole).value<QColor>());
                pen.setWidth(SHAPE_PEN_WIDTH);
                painter.setPen(pen);

                painter.drawEllipse(elRect);
                painter.drawPoint(elRect.center());

            }
        }
    }
    else {
        // Draw standard logo
        QPixmap stdPix = QPixmap(":/images/femto_logo_bg.png");
        this->resize(stdPix.width(), stdPix.height());
        //QRectF target(0, 0, this->width(), this->height());
        QPainter painter(this);
        painter.drawPixmap(0, 0, stdPix);
            
    }
}

void PaintLabel :: mouseReleaseEvent(QMouseEvent *ev) {
    if (drawLine && !image.isNull()) {

        double hFactor = (double) this->height() / (double) image.height();
        double wFactor = (double) this->width() / (double) image.width();

        if (!p1.isNull() && p2.isNull()) {
            p2 = QPointF(
                    ev->x() / wFactor,
                    ev->y() / hFactor
                    );
            emit lineDrawn();
        }
        else if (p1.isNull() || (!p1.isNull() && !p2.isNull())) {
            p1 = QPointF(
                    ev->x() / wFactor,
                    ev->y() / hFactor
                    );
            p2 = QPointF();
        }
        update();
    }
}

void PaintLabel :: clear() {
    // set image to null
    image = QImage();
    shapesDetected = false;

    update();
}

void PaintLabel::setImage(QImage & img)
{
    image = img;
    shapesDetected = false;

    // paint the new image
    adjustSize();
}

void PaintLabel :: setFitWindow (bool bFit) {

    fitWindow = bFit;
    adjustSize();
}

void PaintLabel::adjustSize()
{
    // resize the label to the image size
    if (zoomFactor > 0 && !fitWindow) {
        this->resize(image.width() * zoomFactor, image.height() * zoomFactor);
    }

    // repaint the new size
    update();
}

double PaintLabel :: getZoom () const {
    return zoomFactor;
}

void PaintLabel :: setZoom(double zoom)
{
    if (zoom > 0.0 && zoom != zoomFactor)
    {
        // set new zoom level
        zoomFactor = zoom;

        // resize the paint label widget
        adjustSize();
    }
}

void PaintLabel :: onZoomOut () {
    if (zoomFactor - ZOOM_STEP >= ZOOM_MIN) {
        setZoom(zoomFactor - ZOOM_STEP);
    }
    else {
        setZoom(ZOOM_MIN);
    }
}

void PaintLabel :: onZoomIn () {
    if (zoomFactor + ZOOM_STEP <= ZOOM_MAX) {
        setZoom(zoomFactor + ZOOM_STEP);
    }
    else {
        setZoom(ZOOM_MAX);
    }
}
QImage const * PaintLabel :: getImage () const {
        return &image;
}

bool PaintLabel :: isImageNull () const {
    return image.isNull();
}

void PaintLabel :: setEllipsesList (QListWidget const * list) {
    elList = list;
    update();
}

double PaintLabel :: lineLength() {
    double l;
    if (!p1.isNull() && !p2.isNull()) {
        l = sqrt( (p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y()) );
    }
    else {
        l = -1;
    }
    return l;
}

void PaintLabel :: onToggleViewShapes ( bool toggle ) {
    drawShapes = toggle;
    update();
}

void PaintLabel :: onShapesDetected () {
    shapesDetected = true; 
    update();
}


void PaintLabel :: onToggleDrawLine(bool toggle) { 
    if (toggle)
        this->setCursor(Qt::CrossCursor);
    else
        this->unsetCursor();
    drawLine = toggle;
    update();
}

