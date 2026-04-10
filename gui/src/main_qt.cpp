#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <QDir>
#include <QCoreApplication>
#include "GameBridge.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    // Optional: use a built-in style (Material/Universal/Default)
    QQuickStyle::setStyle("Material");

    QQmlApplicationEngine engine;
    GameBridge bridge;
    engine.rootContext()->setContextProperty("GameBridge", &bridge);
    // qml.qrc embeds Main.qml at :/qml/Main.qml (alias), so load that resource
    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    // If resource wasn't embedded (e.g., during dev), try loading from source tree next to project
    if (engine.rootObjects().isEmpty()) {
        QString altPath = QCoreApplication::applicationDirPath() + QDir::separator() + ".." + QDir::separator() + "qml" + QDir::separator() + "Main.qml";
        QUrl altUrl = QUrl::fromLocalFile(QDir(altPath).canonicalPath());
        engine.load(altUrl);
    }
    return app.exec();
}
