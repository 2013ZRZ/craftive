#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QTranslator>

void                  i18nInit();
static inline QString tr(const char *s, const char *c = nullptr, int n = -1);