#include "drawonwidget.h"
#include <iostream>
#include <QPainter>
#include <QDebug>
#include <qrgb.h>
#include <qxmlstream.h>
#include <QXmlStreamReader>
/**
 * @brief constructs a two-layered sketchpad to draw a simulated
 * path for a robotic arm in.
 * @param parent (QWidget)
 * @param num (int)
 */
drawOnWidget::drawOnWidget(QWidget * parent, int num)
    :QLabel(parent)
{
   prevX = -10;
   prevY = -10;
   pointCount = 0;
   penColor = "black";
   penStyle = "solid";
   penWidth = 4;

   idNumber = num;

   if (idNumber == 0){
	   this->setStyleSheet("background-color:rgba(255,0,0,0);");
   }
   else{
	   this->setStyleSheet("background-color:rgba(0,0,0,255");
   }


}

/**
 * @brief gets the coordinates of clicks and draws lines between them.
 * Also sends points to Line.
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

/**
 * @brief sketches a point into the workspace
 * @param currentX (int)
 * @param currentY (int)
 * @return
 */
bool drawOnWidget::drawPoint(int currentX, int currentY){
    //setup painter and pen
    QImage* temp;
    if(this->pixmap() == 0){
		temp = new QImage(this->width(), this->height(), QImage::Format_ARGB32_Premultiplied);
    }else{
        temp = new QImage(this->pixmap()->toImage());
    }
    if(temp->isNull()){
        std::cout<<"image returned null.  Proabably out of space."<<std::endl;
		return false;
    }
    QPainter painter(temp);


    pointCount ++;
    if(prevX == -10 && prevY == -10){
        prevX = currentX;
        prevY = currentY;

		if (idNumber == 0){
			//draw elipse for first point
			pen.setWidth(2);
			pen.setColor(Qt::blue);
			painter.setPen(pen);
			painter.drawEllipse(QPoint(currentX, currentY), 5, 5);
		}

        //make elipse show up
        this->setPixmap(QPixmap::fromImage(*temp));
        return true;
    }

	QColor color(penColor);
    pen.setColor(color);


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

	if (idNumber == 0){
		if (penColor != "blue"){
			pen.setColor(Qt::blue);
		}
		else{
			pen.setColor("black");
		}
		pen.setWidth(2);
		painter.setPen(pen);
		painter.drawEllipse(QPoint(currentX, currentY), 6, 6); //circle
	}
    //actual drawing//

    //have the label show what is in the image.
    this->setPixmap(QPixmap::fromImage(*temp));

    painter.end();

    delete temp;

	//clicked in roughly same spot twice.
	bool xMatch = (prevX > currentX - 7 && prevX < currentX + 7);
	bool yMatch = (prevY > currentY - 7 && prevY < currentY + 7);
	if (xMatch && yMatch && pointCount > 2){
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

/**
 * erases the sketchpad
 * @param resetBackground (int)
 */
void drawOnWidget::clearAll(int resetBackground){
	prevX = -10;
	prevY = -10;
	pointCount = 0;
	if (resetBackground == 1){
		QImage *temp2 = new QImage(this->width(), this->height(), QImage::Format_ARGB32);
		this->setPixmap(QPixmap::fromImage(*temp2));
	}


}

/**
 * @brief links changes in sketchpad to given editor
 * @param editor (Line)
 */
void drawOnWidget::updateToEditor(Line *editor){
	if (idNumber == 0) clearAll(1);
	if (idNumber == 1) clearAll(0);
    QList<QComboBox *> comboBoxes = editor->CommandEditorWidget->findChildren<QComboBox *>();
    QList<QLineEdit *> lineEdits = editor->CommandEditorWidget->findChildren<QLineEdit *>();
    QList<QSpinBox *> spinBoxes = editor->CommandEditorWidget->findChildren<QSpinBox *>();

    penColor = comboBoxes.at(0)->currentText();
    penStyle = comboBoxes.at(1)->currentText();
    penWidth = spinBoxes.at(0)->text().toInt();

	if (idNumber == 1){
		//switch for the special versions of the colors
		QStringList colors;
		colors << "black" << "orange" << "yellow" << "green" << "red" << "blue" << "purple";

		switch (colors.indexOf(comboBoxes.at(0)->currentText())){
		case (1) :
			penColor = "#FFCD70";
			break;
		case(2) :
			penColor = "#FFFFA3";
			break;
		case(3) :
			penColor = "#71B871";
			break;
		case(4) :
			penColor = "#FF7575";
			break;
		case(5) :
			penColor = "#6E6EB8";
			break;
		case(6) :
			penColor = "#AF5FAF";
			break;
		default:
			penColor = "#999999";
			break;
		}
	}

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

/**
 * @brief fills out the sketchpad with every command
 * @param commandView (CommandViewer)
 */
void drawOnWidget::updateToAllEditors(CommandViewer* commandView, QString projectLocation){
	QListWidget * list = commandView->list;
	clearAll(1);
	std::vector<int> x, y;
	QString lineColor;
	QString lineStyle;
	int currentLineWidth;

	QList<QString> names;
	foreach(QListWidgetItem *item, list->findItems(QString("*"),Qt::MatchWrap | Qt::MatchWildcard)){
		names.append(item->text());
	}
	
	for (int i = 0; i < names.length(); i++){
		clearAll(0);
		//start xml data extraction
		QFile loadFile;
		if (projectName == NULL){
			projectName = "Temp";
		}
		loadFile.setFileName(QString(projectLocation) + QString("/") + names.at(i) + QString(".xml"));
		loadFile.open(QIODevice::ReadOnly);
		QXmlStreamReader reader(&loadFile);

		while (!reader.isEndDocument()) { //loop xml file
			reader.readNext();
			//handles color, linestyle, and thickness lines
			if (reader.name().toString() == "Line"){
				int j = 0;
				foreach(const QXmlStreamAttribute &attr, reader.attributes()){
					if (j == 0){ lineColor = attr.value().toString(); }
					else if (j == 1){ lineStyle = attr.value().toString(); }
					else if (j == 2) { currentLineWidth = attr.value().toInt(); }
					j++;
				} //end line attribute sorting
			} //End line attribute extraction
			
			//handles point lines
			if (reader.name().toString() == "Point"){
				int k = 0;
				foreach(const QXmlStreamAttribute &attr, reader.attributes()){
					if (k == 0) { x.push_back(attr.value().toInt()); }
					if (k == 1) { y.push_back(attr.value().toInt()); }
					k++;
				} //end x/y extraction
			} //end point extraction
		} //End xml data extraction
		//close the file so it can be opened again soon.
		loadFile.close();

		//set pen to correct values

		//switch for the special versions of the colors
		QStringList colors;
		colors << "black" << "orange" << "yellow" << "green" << "red" << "blue" << "purple";

		switch (colors.indexOf(lineColor)){
		case (1) :
			penColor = "#FFCD70";
			break;
		case(2) :
			penColor = "#FFFFA3";
			break;
		case(3) :
			penColor = "#71B871";
			break;
		case(4) :
			penColor = "#FF7575";
			break;
		case(5) :
			penColor = "#6E6EB8";
			break;
		case(6) :
			penColor = "#AF5FAF";
			break;
		default:
			penColor = "#999999";
			break;
		}
		penStyle = lineStyle;
		penWidth = currentLineWidth;

		//draw all the points
		for (int h = 0; h < x.size(); h++){
			drawPoint(x.at(h), y.at(h));
		}
		x.clear();
		y.clear();

	}
}

void drawOnWidget::setWorkSpace(WorkSpace *workSpace){
	this->workSpace = workSpace;
}
