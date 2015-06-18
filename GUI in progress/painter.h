#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <iostream>

namespace Ui {
class Painter;
}

class Painter : public QWidget
{
    Q_OBJECT

public:
    explicit Painter(QWidget *parent = 0);
    ~Painter();
    void paintCommand(int startX, int startY, int endX, int endY, QString color, QString lineStyle);
    void clearPainter();

private:
    Ui::Painter *ui;
    int fudge;

    QColor getPenColor(QString color);
    Qt::PenStyle getPenStyle(QString style);
};

#endif // PAINTER_H
