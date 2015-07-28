#pragma once

#include "CVImageWidget.h"

/**
 * @brief get expected size.
 * @return size
 */
QSize CVImageWidget::sizeHint() {
    return _qimage.size();
}

/**
 * @brief get minimum size.
 * @return size
 */
QSize CVImageWidget::minimumSizeHint() {
    return this->_qimage.size();
}

/**
 * @brief show image as QImage
 * @param image
 */
void CVImageWidget::showImage(const cv::Mat& image)   {
    // Convert the image to the RGB888 format
    switch (image.type()) {
    case CV_8UC1:
		//8 & 4 are ennumerations, replaced with the actual number b/c andrew had difficulties
		//with his library linking correctly.
        cvtColor(image, _tmp, 8);
        break;
    case CV_8UC3:
        cvtColor(image, _tmp, 4);
        break;
    }

    // QImage needs the data to be stored continuously in memory
    //assert(_tmp.isContinuous());
    // Assign OpenCV's image buffer to the QImage. Note that the bytesPerLine parameter
    // (http://qt-project.org/doc/qt-4.8/qimage.html#QImage-6) is 3*width because each pixel
    // has three bytes.
    _qimage = QImage(_tmp.data, _tmp.cols, _tmp.rows, _tmp.cols * 3, QImage::Format_RGB888);

    this->setFixedSize(image.cols, image.rows);

    repaint();
}

/**
 * @brief paint using QPainter.
 */
void CVImageWidget::paintEvent(QPaintEvent* /*event*/) {
    // Display the image
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), _qimage);
    painter.end();
}

/**
 * @brief get mouse clicks.
 * @param event
 */
void CVImageWidget::mousePressEvent(QMouseEvent * event){
    emit emitRefresh(event->localPos().x(), event->localPos().y());
}
