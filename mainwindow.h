#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include <QSystemTrayIcon>
#include <QMenu>

#include "ConfigManager.h"

class Clicker;
class TrayManager;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow *ui = nullptr;
    QMovie* _movie = nullptr;
    TrayManager* _trayManager = nullptr;

    ConfigManager _configManager = ConfigManager();
    bool _isConfigLoaded = false;

    Clicker* _clicker = nullptr;
    unsigned _lastValidMouseButtonIndex = 0;

    void _setAppIcon(const QIcon &icon);

protected:
    void closeEvent(QCloseEvent* event) override;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_comboBoxActivationMode_currentIndexChanged(int index);
    void on_pushButtonActivationKey_clicked();
    void on_comboBoxMouseButton_currentIndexChanged(int index);
    void on_spinBoxClicksPerSecond_valueChanged(int arg1);
    void on_spinBoxClicksPerSecond_editingFinished();
    void on_pushButtonResetSettings_clicked();
    void on_pushButtonExit_clicked();
    void on_pushButtonOk_clicked();
    void on_checkBoxAutostartWithWindows_checkStateChanged(const Qt::CheckState& arg1);
    void on_checkBoxRunInSystemTray_checkStateChanged(const Qt::CheckState& arg1);

signals:
    void iconChanged(const QIcon& icon);
};
#endif // MAINWINDOW_H
