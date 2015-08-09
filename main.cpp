#include <QApplication>
#include <QQmlApplicationEngine>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    /*
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    */
    MainWindow *main = new MainWindow();
    main->show();

    return app.exec();
}
