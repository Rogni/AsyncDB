#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <AsyncDB/QmlAsyncDB.h>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QQmlApplicationEngine engine;

    QmlAsyncDB::registerQmlType(&engine);

    engine.load("qrc:/main.qml");
    if (engine.rootObjects().isEmpty()) return -1;

    return a.exec();
}
