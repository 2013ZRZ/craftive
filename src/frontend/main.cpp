#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

int main(int argc, char *argv[]) {
    QGuiApplication app{argc, argv};
    QTranslator     appTranslator;
    if (appTranslator.load(QLocale::system(), "craftive", "_", ":/i18n/")) {
        qDebug() << QString{"Loaded translator successfully with filePath = %1"}.arg(
            appTranslator.filePath());
        app.installTranslator(&appTranslator);
    } else {
        qWarning() << QString{"Failed to load from these files: "} + [&] -> QString {
            QString result;
            for (const auto &i : QLocale::system().uiLanguages(QLocale::TagSeparator::Underscore))
                result += QString{":/i18n/craftive_%1.qm "}.arg(i);
            return result;
        }();
    }
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
    engine.loadFromModule("crt", "Main");
    return app.exec();
}
