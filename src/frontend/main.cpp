#include "../core/i18n.hpp"
#include <QGuiApplication>
#include <QPluginMetaData>
#include <QQmlApplicationEngine>

extern "C" QObject        *qt_plugin_instance();
extern "C" QPluginMetaData qt_plugin_query_metadata_v2();

namespace {
struct StaticMd3CorePlugin {
    StaticMd3CorePlugin() {
        qRegisterStaticPluginFunction({qt_plugin_instance, qt_plugin_query_metadata_v2});
    }
};
static StaticMd3CorePlugin staticMd3CorePluginInstance;
} // namespace

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
    engine.load(QUrl{"qrc:///qmls/main.qml"});
    return app.exec();
}
