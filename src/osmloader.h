#ifndef OSMLOADER_H
#define OSMLOADER_H

#include <QObject>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

//     z (zoom) — уровень масштабирования (0 = вся Земля на одном тайле, 19 = очень крупно)
//     x — номер тайла по горизонтали (слева направо)
//     y — номер тайла по вертикали (сверху вниз)
class OSMLoader : public QObject
{
    Q_OBJECT
public:
    explicit OSMLoader(QObject *parent = nullptr);
    void loadTile(int x, int y, int z);

signals:
    void tileLoaded(const QPixmap &tile, int x, int y, int z);
    void tileError(const QString &errorString, int x, int y, int z);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
};

#endif // OSMLOADER_H
