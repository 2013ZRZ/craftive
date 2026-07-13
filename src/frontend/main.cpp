#include "../core/i18n.hpp"
#include "md3coreplugin.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

Q_IMPORT_PLUGIN(Md3CorePlugin)

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(md3Core);

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
    return app.exec();
}
