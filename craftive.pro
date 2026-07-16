TEMPLATE = app

CONFIG += lrelease

QMAKE_CC           = clang-22
QMAKE_CXX          = clang++-22
QMAKE_LINK         = clang++-22
QMAKE_LINK_SHLIB   = clang++-22
QMAKE_AR           = llvm-ar-22
QMAKE_RANLIB       = llvm-ranlib-22
QMAKE_CXXFLAGS    += -flto -std=c++26 -stdlib=libc++ -Wno-unused-parameter
QMAKE_LFLAGS      += -flto -fuse-ld=lld
QMAKE_MOC_OPTIONS += -Muri=md3.Core

QT += quick

INCLUDEPATH += include/backward-cpp include/material-components-qml
LIBS        += lib/libmd3core.a

SOURCES = $$files(src/core/*.cpp) $$files(src/frontend/*.cpp)

DESTDIR                  = out
TARGET                   = craftive
TRANSLATIONS             = $$files(locales/craftive_*.ts)
LRELEASE_DIR             = out/locales

RESOURCES += res.qrc
RCC_DIR    = ../.cache/rcc

MOC_DIR     = .cache/moc
OBJECTS_DIR = .cache/o