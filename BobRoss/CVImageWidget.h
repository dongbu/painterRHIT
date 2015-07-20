#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include <QMouseEvent>

class CVImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CVImageWidget(QWidget *parent = 0) : QWidget(parent) {}
    QSize sizeHint();
    QSize minimumSizeHint();

public slots :

    void showImage(const cv::Mat& image);

protected:
    void paintEvent(QPaintEvent* /*event*/);
    void mousePressEvent(QMouseEvent * event);

    QImage _qimage;
    cv::Mat _tmp;

signals:
    void emitRefresh(int currentX, int currentY);
};
