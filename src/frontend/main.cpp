#include "settings.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>

int main(int argc, char *argv[]) {
    // Create application
    QGuiApplication app{argc, argv};
    QTranslator     appTranslator;
    if (appTranslator.load(
            SettingsInstance().value("appearance/locale", QLocale::system()).toLocale(),
            "craftive",
            "_",
            ":/i18n/"))
        app.installTranslator(&appTranslator);

    // Create QML engine
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [] {
            qFatal() << "Received signal QQmlApplicationEngine::objectCreationFailed";
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    // Expose buildinfo to QML
    engine.rootContext()->setContextProperty("buildInfoBuildTime", QString{BUILDINFO_BUILDTIME});
    engine.rootContext()->setContextProperty("buildInfoCommitHash", QString{BUILDINFO_COMMITHASH});

    // Start!!
    engine.loadFromModule("crt", "Main");
    return app.exec();
}
