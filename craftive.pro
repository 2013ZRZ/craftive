TEMPLATE = app

CONFIG += lrelease embed_translations

QMAKE_CC         = clang-22
QMAKE_CXX        = clang++-22
QMAKE_LINK       = clang++-22
QMAKE_LINK_SHLIB = clang++-22
QMAKE_AR         = llvm-ar-22
QMAKE_RANLIB     = llvm-ranlib-22
QMAKE_CXXFLAGS  += -flto -std=c++26 -fuse-ld=lld -stdlib=libc++
QMAKE_LFLAGS    += -flto

QT += quick

INCLUDEPATH += include/backward-cpp include/material-components-qml
LIBS        += lib/libmd3core.a

SOURCES = $$files(src/core/*.cpp) $$files(src/frontend/*.cpp)

DESTDIR                  = out
TARGET                   = craftive
TRANSLATIONS             = $$files(locales/craftive_*.ts)
LRELEASE_DIR             = locales
QM_FILES_RESOURCE_PREFIX = /i18n

RESOURCES += res.qrc

MOC_DIR     = .cache/moc
OBJECTS_DIR = .cache/o