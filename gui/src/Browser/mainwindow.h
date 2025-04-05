#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WebServer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(HttpsWebServer* server, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleButtonClick();
    void updateLog(const QString& message);
private:
    HttpsWebServer* server_;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
