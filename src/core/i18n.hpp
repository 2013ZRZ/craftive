#pragma once

#include <QtCore/QCoreApplication>

void i18nInit();

static inline QString
translate(const char *context, const char *key, const char *disambiguation = nullptr, int n = -1) {
    return QCoreApplication::translate(context, key, disambiguation, n);
}