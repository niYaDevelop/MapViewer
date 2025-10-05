## Возможности приложения
- Загрузка тайла карты OpenStreetMap по заданным координатам
- Отображение координат под курсором в WGS84 и UTM
<img width="250" height="300" alt="screenshot1" src="https://github.com/user-attachments/assets/3c29a5be-5896-462d-8580-cddb0a846a94" />

## Готовая сборка
https://github.com/niYaDevelop/MapViewer/releases/tag/v1.0

## Как собрать приложение на Linux
- Установите Qt если не установлен:
```
sudo apt update
sudo apt install build-essential git wget
sudo apt install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
sudo apt install qt5-default
qmake --version
```
- Соберите проект используя qmake:
```
mkdir build && cd build
qmake ../MapViewer.pro
make 
```
