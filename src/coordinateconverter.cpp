#include "coordinateconverter.h"
#include <cmath>


void CoordinateConverter::wgs84ToUtm(double lat, double lon, int &zone, double &easting, double &northing) {
    if (lat < -80.0 || lat > 84.0) {
        // UTM не определён за пределами этих широт
        zone = 0;
        easting = northing = 0.0;
        return;
    }

    zone = static_cast<int>(floor((lon + 180.0) / 6.0)) + 1;

    // Центральный меридиан зоны
    double lon0 = (zone - 1) * 6.0 - 180.0 + 3.0;

    double latRad = lat * M_PI / 180.0;
    double lonRad = lon * M_PI / 180.0;
    double lon0Rad = lon0 * M_PI / 180.0;

    // Константы эллипсоида
    double a = WGS84_A;
    double e2 = WGS84_E2;

    double N = a / sqrt(1 - e2 * sin(latRad) * sin(latRad));
    double T = tan(latRad) * tan(latRad);
    double C = e2 / (1 - e2) * cos(latRad) * cos(latRad);
    double A = (lonRad - lon0Rad) * cos(latRad);

    double M = a * ((1 - e2 / 4.0 - 3 * e2 * e2 / 64.0 - 5 * e2 * e2 * e2 / 256.0) * latRad
                    - (3 * e2 / 8.0 + 3 * e2 * e2 / 32.0 + 45 * e2 * e2 * e2 / 1024.0) * sin(2 * latRad)
                    + (15 * e2 * e2 / 256.0 + 45 * e2 * e2 * e2 / 1024.0) * sin(4 * latRad)
                    - (35 * e2 * e2 * e2 / 3072.0) * sin(6 * latRad));

    easting = N * A + N * A * A * A * (1 - T + C) / 6.0
              + N * A * A * A * A * A * (5 - 18 * T + T * T + 72 * C - 58 * e2) / 120.0;
    easting += 500000.0;

    northing = M + N * tan(latRad) * (A * A / 2.0
                                      + A * A * A * A * (5 - T + 9 * C + 4 * C * C) / 24.0
                                      + A * A * A * A * A * A * (61 - 58 * T + T * T + 600 * C - 330 * e2) / 720.0);

    if (lat < 0) {
        northing += 10000000.0;
    }
}

QPoint CoordinateConverter::wgs84ToTile(double lat, double lon, int zoom) {
    if (zoom < 0 || zoom > 25) {
        return QPoint(-1, -1);
    }
    while (lon < -180.0) lon += 360.0;
    while (lon >= 180.0) lon -= 360.0;

    int n = 1 << zoom; // 2^zoom

    int x = static_cast<int>(floor((lon + 180.0) / 360.0 * n));
    x = qBound(0, x, n - 1);

    double latRad = lat * M_PI / 180.0;
    double y = (1.0 - log(tan(latRad) + 1.0 / cos(latRad)) / M_PI) / 2.0 * n;
    int yInt = static_cast<int>(floor(y));
    yInt = qBound(0, yInt, n - 1);
    return QPoint(x, yInt);
}

QPointF CoordinateConverter::tilePixelToWgs84(int tileX, int tileY, int zoom, int px, int py) {
    if (zoom < 0 || zoom > 19 || px < 0 || px >= 256 || py < 0 || py >= 256)
        return QPointF(0, 0);

    double n = static_cast<double>(1 << zoom);
    double lon = (static_cast<double>(tileX) + static_cast<double>(px) / 256.0) / n * 360.0 - 180.0;

    double latRad = atan(sinh(M_PI * (1.0 - 2.0 * (static_cast<double>(tileY) + static_cast<double>(py) / 256.0) / n)));
    double lat = latRad * 180.0 / M_PI;

    return QPointF(lon, lat);
}
