TEMPLATE = app

CONFIG += lrelease embed_translations

QMAKE_CC         = clang
QMAKE_CXX        = clang++
QMAKE_LINK       = clang++
QMAKE_LINK_SHLIB = clang++
QMAKE_AR         = llvm-ar
QMAKE_RANLIB     = llvm-ranlib
QMAKE_CXXFLAGS  += -flto -std=c++26
QMAKE_LFLAGS    += -flto

QT += quick

INCLUDEPATH += include/backward-cpp

SOURCES = $$files(src/core/*.cpp) $$files(src/frontend/*.cpp)

DESTDIR                  = out
TARGET                   = craftive
TRANSLATIONS             = $$files(locales/craftive.*.ts)
LRELEASE_DIR             = locales
QM_FILES_RESOURCE_PREFIX = /i18n

RESOURCES += res.qrc
