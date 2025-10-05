#include "mapwindow.h"
#include "ui_mapwindow.h"
#include "coordinateconverter.h"
#include <QMessageBox>

MapWindow::MapWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MapWindow)
{
    ui->setupUi(this);

    // Отображаем изменние курсора
    connect(ui->mapWidgetUI, &MapWidget::mouseMoved, this, [this](int px, int py) {
        QPointF wgs = CoordinateConverter::tilePixelToWgs84(tileX, tileY, tileZ, px, py);
        double lon = wgs.x();
        double lat = wgs.y();

        int utmZone;
        double easting, northing;
        CoordinateConverter::wgs84ToUtm(lat, lon, utmZone, easting, northing);

        QString title = QString("Координаты | WGS84: %1, %2 | UTM: %3N E=%4 N=%5")
                            .arg(lat, 0, 'f', 6)
                            .arg(lon, 0, 'f', 6)
                            .arg(utmZone)
                            .arg(easting, 0, 'f', 0)
                            .arg(northing, 0, 'f', 0);

        setWindowTitle(title);
    });

    // Загружаем тайл по нажатию кнопки
    connect(ui->updateButtonUI, &QPushButton::pressed, this, [this](){
        int z = ui->zoomUI->value();
        QPoint p = CoordinateConverter::wgs84ToTile(ui->latUI->value(), ui->lonUI->value(), z);
        osmLoader.loadTile(p.x(), p.y(), z);
    });

    // Отображаем загруженный тайл
    connect(&osmLoader, &OSMLoader::tileLoaded, this, [this](const QPixmap &tile, int x, int y, int z){
        ui->mapWidgetUI->setTileImage(tile);
        tileX = x; tileY = y; tileZ = z;
    });

    // Обрабатываем ошибку загрузки тайла
    connect(&osmLoader, &OSMLoader::tileError, this, [this](const QString &errorString, int, int, int) {
        QMessageBox::critical(this, "Ошибка загрузки карты", errorString);
    });
}

MapWindow::~MapWindow()
{
    delete ui;
}
