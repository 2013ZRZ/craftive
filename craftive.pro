TEMPLATE = app

CONFIG += lrelease

QT += qml

HEADERS = include/

SOURCES = src/core/*.cpp src/frontend/*.cpp

DESTDIR = out

TARGET = out/craftive

TRANSLATIONS = locales/craftive.*.ts
