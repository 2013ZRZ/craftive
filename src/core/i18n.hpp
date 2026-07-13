#pragma once

#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>

#define CRAFTIVE_I18N_INIT                                                          \
    QTranslator appTranslator;                                                      \
    if (appTranslator.load("craftive_" + QLocale::system().name(), "qrc:///i18n/")) \
        QCoreApplication::instance()->installTranslator(&appTranslator);

static inline QString
translate(const char *context, const char *key, const char *disambiguation = nullptr, int n = -1) {
    return QCoreApplication::translate(context, key, disambiguation, n);
}