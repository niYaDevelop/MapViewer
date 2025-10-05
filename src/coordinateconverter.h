#ifndef COORDINATECONVERTER_H
#define COORDINATECONVERTER_H

#include <QPointF>
#include <QPoint>

class CoordinateConverter
{
public:
    static void wgs84ToUtm(double lat, double lon, int &zone, double &easting, double &northing);

    // WGS84 -> тайловые координаты (x, y) для заданного zoom
    static QPoint wgs84ToTile(double lat, double lon, int zoom);

    // Пиксель на тайле → WGS84
    // tileX, tileY, zoom — координаты тайла
    // px, py — пиксель внутри тайла (0..255)
    static QPointF tilePixelToWgs84(int tileX, int tileY, int zoom, int px, int py);

private:
    static constexpr double WGS84_A = 6378137.0;          // Полуось
    static constexpr double WGS84_E2 = 0.00669437999014;  // Квадрат эксцентриситета
};

#endif // COORDINATECONVERTER_H
