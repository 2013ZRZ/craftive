#include "i18n.hpp"
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLibraryInfo>
#include <QtCore/QString>

void i18nInit() {
    auto app = QCoreApplication::instance();
    if (!app)
        return;

    QString translationsDir;
#ifdef _CRAFTIVE_DEBUG
#ifdef _CRAFTIVE_DEBUGGING_LOCALES_DIR
    translationsDir = QString::fromUtf8(_CRAFTIVE_DEBUGGING_LOCALES_DIR);
#else
#warning "_CRAFTIVE_DEBUGGING_LOCALES_DIR is undefined; using ./locales as the locales directory"
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

    QString      localeName   = QLocale::system().name();
    QTranslator *qtTranslator = new QTranslator{app};
    if (qtTranslator->load("qt_" + localeName, QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        app->installTranslator(qtTranslator);
    else
        delete qtTranslator;
    QTranslator *appTranslator = new QTranslator{app};
    if (appTranslator->load("craftive." + localeName,
                            translationsDir
#if !(defined(_WIN32) || defined(__APPLE__))
                                + "/" + localeName + "/LC_MESSAGES"
#endif
                            ))
        app->installTranslator(appTranslator);
    else {
        delete appTranslator;
        appTranslator = new QTranslator(app);
        if (appTranslator->load("craftive.en_US",
                                translationsDir
#if !(defined(_WIN32) || defined(__APPLE__))
                                    + "/" + localeName + "/LC_MESSAGES"
#endif
                                ))
            app->installTranslator(appTranslator);
    }
}

static inline QString tr(const char *s, const char *c, int n) {
    return QCoreApplication::tr(s, c, n);
}