#pragma once

#include "QtCore/qcoreapplication.h"
#include <QtCore/QCoreApplication>

void i18nInit();

static inline QString tr(const char *s, const char *c = nullptr, int n = -1) {
    return QCoreApplication::tr(s, c, n);
}

static inline QString
translate(const char *context, const char *key, const char *disambiguation = nullptr, int n = -1) {
    return QCoreApplication::translate(context, key, disambiguation, n);
}