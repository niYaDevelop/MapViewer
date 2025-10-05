#include "mapwidget.h"
#include <QPainter>
#include <QMouseEvent>

MapWidget::MapWidget(QWidget *parent)
    : QWidget{parent} {
    setMouseTracking(true);
}

void MapWidget::setTileImage(const QPixmap &tile) {
    currentTile = tile;
    update();
}

void MapWidget::paintEvent(QPaintEvent *event) {
    (void)event;
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    if (!currentTile.isNull()) {
        QPixmap scaled = currentTile.scaled(
            size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );
        QRect target = QRect(
            (width() - scaled.width()) / 2,
            (height() - scaled.height()) / 2,
            scaled.width(),
            scaled.height()
            );
        painter.drawPixmap(target, scaled);
    }
}

void MapWidget::mouseMoveEvent(QMouseEvent *event) {
    if (currentTile.isNull()) {
        return;
    }

    // Размер исходного тайла
    int origW = currentTile.width();
    int origH = currentTile.height();
    if (origW <= 0 || origH <= 0) return;

    // Размер отрисованного (масштабированного) изображения
    QPixmap scaled = currentTile.scaled(
        size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    int scaledW = scaled.width();
    int scaledH = scaled.height();

    // Позиция левого верхнего угла изображения (из-за центрирования)
    int offsetX = (width() - scaledW) / 2;
    int offsetY = (height() - scaledH) / 2;

    // Позиция мыши относительно изображения в масштабированных пикселях
    int scaledX = event->position().x() - offsetX;
    int scaledY = event->position().y() - offsetY;

    // Проверяем, попадает ли в изображение
    if (scaledX >= 0 && scaledX < scaledW && scaledY >= 0 && scaledY < scaledH) {
        // Преобразуем в координаты исходного тайла
        double ratioX = static_cast<double>(origW) / scaledW;
        double ratioY = static_cast<double>(origH) / scaledH;

        int origX = static_cast<int>(scaledX * ratioX);
        int origY = static_cast<int>(scaledY * ratioY);

        // Ограничиваем диапазон
        origX = qBound(0, origX, origW - 1);
        origY = qBound(0, origY, origH - 1);

        emit mouseMoved(origX, origY);
    }
}
