#ifndef DRAWONWIDGET_H
#define DRAWONWIDGET_H

#include "commandeditor.h"
#include <QMouseEvent>
#include <QLabel>
#include <QPen>
#include <qlistwidget.h>


class drawOnWidget : public QLabel
{
    Q_OBJECT
public:
    drawOnWidget(QWidget * parent, int num);
    ~drawOnWidget(){}
    void clearAll(int resetBackground);
    CommandEditor* currentEditor;
	QString projectName;

private:
    int prevX,prevY,pointCount, penWidth, idNumber;
    QString penColor, penStyle;
    QPen pen;
	QLabel frontLabel;


    bool drawPoint(int currentX, int currentY);

signals:
    void sendPoint(int x, int y, int pointCount);

public slots:
    void updateToEditor(CommandEditor* editor);
	void updateToAllEditors(QListWidget* list);

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // DRAWONWIDGET_H
