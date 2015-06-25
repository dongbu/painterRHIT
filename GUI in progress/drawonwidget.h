#ifndef DRAWONWIDGET_H
#define DRAWONWIDGET_H

#include "commandeditor.h"
#include <QMouseEvent>
#include <QLabel>
#include <QPen>


class drawOnWidget : public QLabel
{
    Q_OBJECT
public:
    drawOnWidget(QWidget * parent);
    ~drawOnWidget(){}
    void clearAll();

private:
    int prevX,prevY,pointCount;
    QString penColor, penStyle;
    QPen pen;

    bool drawPoint(int currentX, int currentY);

signals:
    void sendPoint(int x, int y, int pointCount);

public slots:
    void updateToEditor(CommandEditor* editor);

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // DRAWONWIDGET_H
