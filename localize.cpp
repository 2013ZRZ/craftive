#include "localize.hpp"
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLibraryInfo>
#include <QtCore/QString>

void localizeInit() {
    auto app = QApplication::instance();
    if (!app)
        return;

    QString translationsDir;
#ifdef _CRAFTIVE_DEBUG
#ifdef _CRAFTIVE_DEBUGGING_LOCALES_DIR
    translationsDir = QString::fromUtf8(_CRAFTIVE_DEBUGGING_LOCALES_DIR);
#else
    translationsDir = QDir::currentPath() + "/locales";
#endif
#else
#if defined(_WIN32) || defined(__APPLE__)
    QString   execPath = QApplication::applicationFilePath();
    QFileInfo fi{execPath};
    QDir      dir = fi.absoluteDir();
#if defined(_WIN32)
    translationsDir = dir.absolutePath() + "/locales";
#elif defined(__APPLE__)
    dir.cdUp();
    translationsDir = dir.absolutePath() + "/Resources/locales";
#endif
#else
    translationsDir = "/usr/share/locale";
#endif
#endif

    QTranslator *qtTranslator = new QTranslator{app};
    if (qtTranslator->load("qt_" + QLocale::system().name(),
                           QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        app->installTranslator(qtTranslator);
    else
        delete qtTranslator;
    QTranslator *appTranslator = new QTranslator{app};
    QString      localeName    = QLocale::system().name();
    if (appTranslator->load("craftive_" + localeName, translationsDir))
        app->installTranslator(appTranslator);
    else {
        delete appTranslator;
        appTranslator = new QTranslator(app);
        if (appTranslator->load("craftive_en_US", translationsDir))
            app->installTranslator(appTranslator);
    }
}