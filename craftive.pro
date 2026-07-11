TEMPLATE = app

CONFIG += lrelease

QT += qml

SOURCES = src/core/*.cpp src/frontend/*.cpp

DESTDIR = out

TARGET = out/craftive

TRANSLATIONS = locales/craftive.*.ts