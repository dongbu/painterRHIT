#include "drawonwidget.h"
#include <iostream>
#include <QPainter>
#include <QDebug>

drawOnWidget::drawOnWidget(QWidget * parent)
    :QLabel(parent)
{
   prevX = -10;
   prevY = -10;
   pointCount = 0;
   penColor = "black";
   penStyle = "solid";
   penWidth = 4;
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

    if(drawPoint(currentX,currentY)){
        emit sendPoint(currentX, currentY, pointCount);
    }
}

bool drawOnWidget::drawPoint(int currentX, int currentY){
    //setup painter and pen
    QImage* temp;
    if(this->pixmap() == 0){
        temp = new QImage(this->width(),this->height(),QImage::Format_ARGB32);
    }else{
        temp = new QImage(this->pixmap()->toImage());
    }
    if(temp->isNull()){
        std::cout<<"image is somehow bad."<<std::endl;
    }
    QPainter painter(temp);


    pointCount ++;
    if(prevX == -10 && prevY == -10){
        prevX = currentX;
        prevY = currentY;

        //draw elipse for first point
        pen.setWidth(2);
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawEllipse(QPoint(currentX,currentY),5,5);

        //make elipse show up
        this->setPixmap(QPixmap::fromImage(*temp));
        return true;
    }

    pen.setColor(penColor);
    QStringList styles;
    styles << "solid" << "dashed" << "dashed dot";
    switch(styles.indexOf(penStyle)){
    case 0:
        pen.setStyle(Qt::SolidLine);
        break;
    case 1:
        pen.setStyle(Qt::DashLine);
        break;
    case 2:
        pen.setStyle(Qt::DashDotLine);
        break;
    default:
        pen.setStyle(Qt::SolidLine);
        break;
    }

    pen.setWidth(penWidth);
    painter.setPen(pen);


    //actual drawing//
    painter.drawLine(QPointF(prevX,prevY),QPointF(currentX,currentY)); //line

    if(penColor != "blue"){
        pen.setColor(Qt::blue);
    }else{
        pen.setColor("black");
    }
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawEllipse(QPoint(currentX,currentY),6,6); //circle
    //actual drawing//

    //have the label show what is in the image.
    this->setPixmap(QPixmap::fromImage(*temp));

    painter.end();

    delete temp;

    //clicked in roughly same spot twice.
    bool xMatch = (prevX > currentX - 7 && prevX < currentX + 7);
    bool yMatch = (prevY > currentY - 7 && prevY < currentY + 7);
    if(xMatch && yMatch && pointCount > 2){
        //QImage *temp2 = new QImage(this->width(),this->height(),QImage::Format_ARGB32);
        //this->setPixmap(QPixmap::fromImage(*temp2));
        currentX = -10;
        currentY = -10;
        emit sendPoint(currentX, currentY, pointCount);
        pointCount = 0;
        prevX = currentX;
        prevY = currentY;
        return false;
    }
    prevX = currentX;
    prevY = currentY;
    return true;
}

void drawOnWidget::clearAll(int resetBackground){
	prevX = -10;
	prevY = -10;
	pointCount = 0;
	if (resetBackground == 1){
		printf("clear the background\n");
		QImage *temp2 = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
		this->setPixmap(QPixmap::fromImage(*temp2));
		this->penColor = "black";
		this->penStyle = Qt::SolidLine;
		this->penWidth = 4;
	}
	else if(resetBackground == 2){
		printf("fade the background\n");
		QRgb colorVal;
		QImage *temp2 = new QImage(this->pixmap()->toImage());
		for (int p1 = 0; p1 < temp2->width(); p1++){
			for (int p2 = 0; p2 < temp2->height(); p2++){
				colorVal = temp2->pixel(p1, p2);
				QColor color(colorVal);
				color = color.light(0);
				QRgb newColor = color.rgb();
				temp2->setPixel(p1, p2, newColor);
				//int newRed = color.red() + 10;
				//int newGreen = color.green() + 10;
				//int newBlue = color.blue() + 10;
				//if (newRed < 200 && newGreen < 200 && newBlue < 200){
				//	printf("red: %i, green: %i, blue: %i", newRed, newGreen, newBlue);
				//	temp2->setPixel(p1, p2, qRgb(newRed, newGreen, newBlue));
				//}

			}
		}
		temp2->create
		this->setPixmap(QPixmap::fromImage(*temp2));
	}
	else{
		printf("do nothing to background\n");

	}
    //this->clear();
   /* this->penColor = "black";
    this->penStyle = Qt::SolidLine;
    this->penWidth = 4;*/
}

void drawOnWidget::updateToEditor(CommandEditor *editor){
    clearAll(0);
    QList<QComboBox *> comboBoxes = editor->CommandEditorWidget->findChildren<QComboBox *>();
    QList<QLineEdit *> lineEdits = editor->CommandEditorWidget->findChildren<QLineEdit *>();
    QList<QSpinBox *> spinBoxes = editor->CommandEditorWidget->findChildren<QSpinBox *>();

    penColor = comboBoxes.at(0)->currentText();
    penStyle = comboBoxes.at(1)->currentText();
    penWidth = spinBoxes.at(0)->text().toInt();

    foreach(QLineEdit *line, lineEdits){
        QString s = line->text();
        QStringList list = s.split(',');
        if(list.length() == 2){
            int x = list.at(0).toInt();
            int y = list.at(1).toInt();
            drawPoint(x,y);
        }

    }
    this->currentEditor = editor;
}
