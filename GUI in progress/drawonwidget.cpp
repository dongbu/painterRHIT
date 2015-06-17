#include "drawonwidget.h"
#include <iostream>
#include <QPainter>
#include <QPen>


drawOnWidget::drawOnWidget(QWidget * parent)
    :QLabel(parent)
{
   prevX = -10;
   prevY = -10;
   pointCount = 0;

}



/**
 * @brief gets the coordinates of clicks and draws lines between them.
 * Also sends points to commandeditor.
 * clicking same spot twice clears area.
 * @param event
 */
void drawOnWidget::mousePressEvent(QMouseEvent * event){
    //get mouse coordinates
    int currentX = event->localPos().x();
    int currentY = event->localPos().y();

    //setup painter and pen
    QImage* temp;
    if(this->pixmap() == 0){
        temp = new QImage(this->width(),this->height(),QImage::Format_ARGB32);
    }else{
        temp = new QImage(this->pixmap()->toImage());
    }
    if(temp->isNull()){
        std::cout<<"image is somehow bad. ='( time to be sad."<<std::endl;
    }
    QPainter painter(temp);
    QPen pen;


    pointCount ++;
    if(prevX == -10 && prevY == -10){
        prevX = currentX;
        prevY = currentY;
        emit sendPoint(currentX, currentY, pointCount);
        //draw elipse for first point
        pen.setWidth(2);
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawEllipse(QPoint(currentX,currentY),5,5);

        //make elipse show up
        this->setPixmap(QPixmap::fromImage(*temp));
        return;
    }


    pen.setColor(Qt::black);

    pen.setWidth(5);
    painter.setPen(pen);

    //actual drawing//
    painter.drawLine(QPointF(prevX,prevY),QPointF(currentX,currentY)); //line

    pen.setColor(Qt::blue);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawEllipse(QPoint(currentX,currentY),6,6); //circle
    //actual drawing//

    //have the label show what is in the image.
    this->setPixmap(QPixmap::fromImage(*temp));

    //clicked in roughly same spot twice.
    bool xMatch = (prevX > currentX - 7 && prevX < currentX + 7);
    bool yMatch = (prevY > currentY - 7 && prevY < currentY + 7);
    if(xMatch && yMatch){
        QImage *temp2 = new QImage(this->width(),this->height(),QImage::Format_ARGB32);
        this->setPixmap(QPixmap::fromImage(*temp2));
        currentX = -10;
        currentY = -10;
        emit sendPoint(currentX, currentY, pointCount);
        pointCount = 0;
        prevX = currentX;
        prevY = currentY;
        return;
    }
    prevX = currentX;
    prevY = currentY;

    emit sendPoint(currentX, currentY, pointCount);

}

void drawOnWidget::clearAll(){
    prevX = -10;
    prevY = -10;
    pointCount = 0;
    QImage *temp2 = new QImage(this->width(),this->height(),QImage::Format_ARGB32);
    this->setPixmap(QPixmap::fromImage(*temp2));

}

