#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QPixmap>

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = nullptr);
    void setTileImage(const QPixmap &tile);

signals:
    // x, y — координаты в пикселях относительно левого верхнего угла изображения
    void mouseMoved(int x, int y);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QPixmap currentTile;
};

#endif // MAPWIDGET_H
