#include "traymanager.h"
#include <QApplication>
#include <QAction>

TrayManager::TrayManager(QMainWindow *window, QObject *parent)
    : QObject(parent), _mainWindow(window) {

    _trayIcon = new QSystemTrayIcon(this);
    _trayIcon->setIcon(_mainWindow->windowIcon());

    _trayMenu = new QMenu(_mainWindow);
    QAction *restoreAction = _trayMenu->addAction("Развернуть");
    QAction *quitAction = _trayMenu->addAction("Выход");

    connect(restoreAction, &QAction::triggered, _mainWindow, &QWidget::showNormal);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    _trayIcon->setContextMenu(_trayMenu);
    _trayIcon->show();

    // Клик левой кнопкой мыши по иконке
    connect(_trayIcon, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            if (_mainWindow->isVisible()) {
                _mainWindow->hide();
            }
            else {
                _mainWindow->showNormal();
                _mainWindow->activateWindow();
            }
        }
    });
}

TrayManager::~TrayManager() {  delete _trayMenu; }

void TrayManager::showMessage(const QString &title, const QString &message) {
    if (_trayIcon) {
        _trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 2000);
    }
}

void TrayManager::setTrayIcon(const QIcon &icon) { if (_trayIcon) _trayIcon->setIcon(icon);  }