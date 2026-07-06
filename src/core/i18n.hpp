#pragma once

#include <QtCore/QCoreApplication>

void i18nInit();

static inline QString tr(const char *s, const char *c = nullptr, int n = -1) {
    return QCoreApplication::tr(s, c, n);
}