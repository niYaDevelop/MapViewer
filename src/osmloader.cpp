#include "osmloader.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QPixmap>

OSMLoader::OSMLoader(QObject *parent)
    : QObject{parent}, networkManager(new QNetworkAccessManager(this)) {
    connect(networkManager, &QNetworkAccessManager::finished, this, &OSMLoader::onReplyFinished);
}

void OSMLoader::loadTile(int x, int y, int z) {
    if (z < 0 || z > 19) {
        emit tileError("Zoom level must be between 0 and 19", x, y, z);
        return;
    }

    QString url = QString("https://tile.openstreetmap.org/%1/%2/%3.png").arg(z).arg(x).arg(y);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Qt-OSM-Loader/1.0 (+https://your-project.example.com)");
    networkManager->get(request);
}

void OSMLoader::onReplyFinished(QNetworkReply *reply) {
    QUrl url = reply->url();
    // Извлекаем x, y, z из URL
    QString path = url.path();
    QStringList parts = path.split('/');
    if (parts.size() < 4) {
        emit tileError("Invalid URL format", -1, -1, -1);
        reply->deleteLater();
        return;
    }

    bool ok;
    int z = parts[1].toInt(&ok); if (!ok) z = -1;
    int x = parts[2].toInt(&ok); if (!ok) x = -1;
    int y = parts[3].split('.')[0].toInt(&ok); if (!ok) y = -1;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QPixmap pixmap;
        if (pixmap.loadFromData(data, "PNG")) {
            emit tileLoaded(pixmap, x, y, z);
        } else {
            emit tileError("Failed to decode image", x, y, z);
        }
    } else {
        emit tileError(reply->errorString(), x, y, z);
    }

    reply->deleteLater();
}
