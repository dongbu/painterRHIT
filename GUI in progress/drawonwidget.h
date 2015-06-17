#ifndef DRAWONWIDGET_H
#define DRAWONWIDGET_H

#include <QMouseEvent>
#include <QLabel>


class drawOnWidget : public QLabel
{
    Q_OBJECT
public:
    drawOnWidget(QWidget * parent);
    ~drawOnWidget(){}
    void clearAll();

private:
    int prevX,prevY,pointCount;

signals:
    void sendPoint(int x, int y, int pointCount);


protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // DRAWONWIDGET_H
