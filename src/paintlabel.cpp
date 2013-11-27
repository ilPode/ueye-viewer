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
}

void PaintLabel::setPixmap(const QPixmap &pixmap)
{
    // we also can draw a pixmap
    // conversion is slow
    image = pixmap.toImage();

    adjustSize();
}

void PaintLabel::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);

    if (!image.isNull())
    {

  /*      int width = image.width() * zoomFactor;
        int height = image.height() * zoomFactor;
        if (zoomFactor <= 0) {
            QSize imgSize = image.size();
            imgSize.scale(this->width(), this->height(), Qt::KeepAspectRatio);
            width = imgSize.width();
            height = imgSize.height();
        }
*/
        int width = this->width();
        int height = this->height();
        qreal posX = 0.0; 
        qreal posY = 0.0;

        // Alignment draw
/*        if (this->alignment() & Qt::AlignCenter)
        {
            posX = this->width() / 2 - image.width() / 2;
            posY = this->height() / 2 - image.height() / 2;
        }
*/
        QRectF target(posX, posY, width, height);

        // draw the image
        QPainter painter(this);
        painter.drawImage(target, image);
        
        // draw detected shapes
        if (drawShapes) {
            painter.setBrush(Qt::NoBrush);
            QListWidgetItem * ellipse;
            double hFactor = (double) height / (double) image.height();
            double wFactor = (double) width / (double) image.width();
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
}

void PaintLabel :: clear() {
    // set image to null
    image = QImage();

    update();
}

void PaintLabel::setImage(QImage & img)
{
    image = img;

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

bool PaintLabel :: isImagePresent () const {
    return (image.isNull())? false : true;
}

void PaintLabel :: setEllipsesList (QListWidget const * list) {
    elList = list;
    update();
}

void PaintLabel :: onToggleViewShapes ( bool toggle ) {
    drawShapes = toggle;
}

