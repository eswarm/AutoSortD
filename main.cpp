#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "dirwatcher.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    qmlRegisterSingletonType<DirWatcher>("eswarm.DirWatcher", 1, 0, "DirWatcher", DirWatcher::singletontype_provider);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

     //DirWatcher *watcher = new DirWatcher();
     //watcher->sortDirectory("D:/downloads");

    /*
    MainWindow *main = new MainWindow();
    main->show();
    */

    return app.exec();
}
