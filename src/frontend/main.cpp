#include "../core/i18n.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    QGuiApplication app{argc, argv};
    CRAFTIVE_I18N_INIT
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
    engine.load(QUrl{"qrc:///qmls/main.qml"});
    return app.exec();
}
