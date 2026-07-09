#include "i18n.hpp"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QTranslator>


namespace {
static QTranslator &appTranslator() {
    static QTranslator inst;
    return inst;
}
} // namespace

void i18nInit() {
    auto app = QCoreApplication::instance();
    if (!app)
        return;

    QString localeName = QLocale::system().name();

    QString translationsDir;
#ifdef _CRAFTIVE_DEBUG
#ifdef _CRAFTIVE_DEBUGGING_LOCALES_DIR
    translationsDir = QString::fromUtf8(_CRAFTIVE_DEBUGGING_LOCALES_DIR);
#else
#error "_CRAFTIVE_DEBUGGING_LOCALES_DIR is undefined"
#endif
#else
#if defined(_WIN32) || defined(__APPLE__)
    translationsDir = QDir(QApplication::applicationDirPath()).absolutePath();
#if defined(__APPLE__)
    translationsDir += "/Contents/Resources/locales";
#else
    translationsDir += "/locales";
#endif
#else
    translationsDir = "/usr/share/locale/" + localeName + "/LC_MESSAGES";
#endif
#endif

    if (appTranslator().load("craftive." + localeName, translationsDir))
        app->installTranslator(&appTranslator());
}