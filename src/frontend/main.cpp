#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QGuiApplication app{argc, argv};
    QTranslator     appTranslator;
    if (appTranslator.load("craftive_" + QLocale::system().name(), "qrc:///i18n/"))
        app.installTranslator(&appTranslator);
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
    engine.load(QUrl{"qrc:///qmls/Main.qml"});
    return app.exec();
}
