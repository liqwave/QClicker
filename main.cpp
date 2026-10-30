#include "mainwindow.h"

#include <shobjidl.h>
#pragma comment(lib, "shell32.lib")

#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
    SetCurrentProcessExplicitAppUserModelID(L"liqwave.QClicker");
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.run();
    return QCoreApplication::exec();
}
