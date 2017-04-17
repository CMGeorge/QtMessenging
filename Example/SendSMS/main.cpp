#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "qtmessenging.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QtMessenging *qMessenging = new QtMessenging(&app);
    qMessenging->sendMessage("0771796123","Mesaj Test Android");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
