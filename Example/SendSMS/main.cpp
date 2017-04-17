#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "qtmessenging.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QtMessenging *qMessenging = new QtMessenging(&app);
    engine.rootContext()->setContextProperty("QtMessenging",qMessenging);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    

    //qMessenging->sendMessage("0365400401","Mesaj Test Android");
    

    return app.exec();
}
