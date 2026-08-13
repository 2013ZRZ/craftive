#include "err.hpp"
#include "settingshelper.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>

CrtExceptReceiver *globalExceptReceiver{nullptr};

int main(int argc, char *argv[]) {
    // Create application
    QGuiApplication app{argc, argv};
    QTranslator     appTranslator;
    if (appTranslator.load(
            SettingsHelper{}.value("appearance/locale", QLocale::system()).toLocale(),
            "craftive",
            "_",
            ":/i18n/"))
        app.installTranslator(&appTranslator);

    // Create exceptions receiver
    CrtExceptReceiver receiver;
    globalExceptReceiver = &receiver;

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

    // Expose the exception receiver to QML
    engine.rootContext()->setContextProperty("exceptReceiver", &receiver);

    // Start!!
    engine.loadFromModule("crt", "Main");
    return app.exec();
}
