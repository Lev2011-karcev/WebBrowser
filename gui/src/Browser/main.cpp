#include "mainwindow.h"
#include "WebServer.h"
#include <QtWidgets>
#include <QWidgetSet>
int main(int argc, char *argv[])
{
    HttpsServer server(
        8080,
        "./html",
        "./certs/server.crt",
        "./certs/server.key",
        "./certs/dh.pem"
    );
    QApplication a(argc, argv);
    MainWindow w(&server);
    w.show();
    QThread* serverThread = new QThread;
    QObject::connect(serverThread, &QThread::started, [&server]() {
        server.start();
    });
    server.moveToThread(serverThread);
    serverThread->start();
    return a.exec();
}
