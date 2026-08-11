#include "mainwindow.h"

#include <shobjidl.h>
#pragma comment(lib, "shell32.lib")

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    SetCurrentProcessExplicitAppUserModelID(L"YourName.QClicker.1");
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.show();
    return QCoreApplication::exec();
}
