#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMainWindow>

class TrayManager : public QObject {
    Q_OBJECT

private:
    QSystemTrayIcon* _trayIcon = nullptr;
    QMenu* _trayMenu = nullptr;
    QMainWindow* _mainWindow = nullptr;

public:
    explicit TrayManager(QMainWindow *mainWindow, QObject *parent = nullptr);
    ~TrayManager() override;

    void showMessage(const QString &title, const QString &message);
    bool isVisible() const { return _trayIcon && _trayIcon->isVisible(); }

    void setTrayIcon(const QIcon &icon);
};

#endif // TRAYMANAGER_H
