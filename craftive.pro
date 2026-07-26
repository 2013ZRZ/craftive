TEMPLATE = app

CONFIG += lrelease embed_translations

QMAKE_CC           = clang-22
QMAKE_CXX          = clang++-22
QMAKE_LINK         = clang++-22
QMAKE_LINK_SHLIB   = clang++-22
QMAKE_AR           = llvm-ar-22
QMAKE_RANLIB       = llvm-ranlib-22
QMAKE_CXXFLAGS    += -g -flto -std=c++26 -stdlib=libc++ -Wno-unused-parameter
QMAKE_LFLAGS      += -g -flto -fuse-ld=lld

QT += quick

INCLUDEPATH += include/backward-cpp \
    include/md3-qml/src/Core/Cpp \
    include/md3-qml/3rdparty/material-color-utilities

CONFIG += qmltypes
QML_IMPORT_NAME = md3.Core
QML_IMPORT_MAJOR_VERSION = 1

MCU_DIR = include/md3-qml/3rdparty/material-color-utilities
MCU_SOURCES = \
    $$MCU_DIR/blend/blend.cc \
    $$MCU_DIR/cam/cam.cc \
    $$MCU_DIR/cam/hct.cc \
    $$MCU_DIR/cam/hct_solver.cc \
    $$MCU_DIR/cam/viewing_conditions.cc \
    $$MCU_DIR/contrast/contrast.cc \
    $$MCU_DIR/dislike/dislike.cc \
    $$MCU_DIR/dynamiccolor/dynamic_color.cc \
    $$MCU_DIR/dynamiccolor/dynamic_scheme.cc \
    $$MCU_DIR/dynamiccolor/material_dynamic_colors.cc \
    $$MCU_DIR/palettes/tones.cc \
    $$MCU_DIR/quantize/celebi.cc \
    $$MCU_DIR/quantize/lab.cc \
    $$MCU_DIR/quantize/wsmeans.cc \
    $$MCU_DIR/quantize/wu.cc \
    $$MCU_DIR/scheme/scheme_content.cc \
    $$MCU_DIR/scheme/scheme_expressive.cc \
    $$MCU_DIR/scheme/scheme_fidelity.cc \
    $$MCU_DIR/scheme/scheme_fruit_salad.cc \
    $$MCU_DIR/scheme/scheme_monochrome.cc \
    $$MCU_DIR/scheme/scheme_neutral.cc \
    $$MCU_DIR/scheme/scheme_rainbow.cc \
    $$MCU_DIR/scheme/scheme_tonal_spot.cc \
    $$MCU_DIR/scheme/scheme_vibrant.cc \
    $$MCU_DIR/score/score.cc \
    $$MCU_DIR/temperature/temperature_cache.cc \
    $$MCU_DIR/utils/utils.cc

SOURCES = $$files(src/core/*.cpp) \
    $$files(src/frontend/*.cpp) \
    include/md3-qml/src/Core/Cpp/stylemanager.cpp \
    $$MCU_SOURCES

DESTDIR                  = out
TARGET                   = craftive
TRANSLATIONS             = $$files(locales/craftive_*.ts)
LRELEASE_DIR             = out/locales
QM_FILES_RESOURCE_PREFIX = /i18n

RESOURCES += res.qrc md3-core.qrc
RCC_DIR    = ../.cache/rcc

MOC_DIR     = .cache/moc
OBJECTS_DIR = .cache/o