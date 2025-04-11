#include "mainwindow.h"
#include "../../../server/include/WebServer.h"
#include <QWidget>
#include <QWidgetSet>
int main(int argc, char *argv[])
{
    HttpsWebServer server(
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
