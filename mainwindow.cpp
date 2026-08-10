#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "KeyBindDialog.h"
#include "StyleManager.h"

#include <QSettings>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/sources/iconDefault.ico"));

    ui->labelClickerStateValue->setAlignment(Qt::AlignCenter);
    ui->labelWarningValue->setAlignment(Qt::AlignCenter);
    StyleManager::setStyleState(ui->labelClickerStateValue, Clicker::ClickerState::BindingKey);
    StyleManager::setStyleWarning(ui->labelWarningValue, Clicker::Warning::ZeroCps);

    _movie = new QMovie(":/sources/sigma.gif", QByteArray(), this);
    ui->labelSigmaGif->setMovie(_movie);
    _movie->start();

    _clicker = new Clicker(this);

    connect(_clicker, &Clicker::modeChanged, this, [this](Clicker::ActivationMode mode) {
        ui->comboBoxActivationMode->setCurrentIndex(static_cast<int>(mode));
        if (_isLoadingConfig) _configManager.write(*_clicker);
    });

    connect(_clicker, &Clicker::bindChanged, this, [this](InputBinding bind) {
        ui->pushButtonActivationKey->setText(bind.toString());
        if (_isLoadingConfig) _configManager.write(*_clicker);
    });

    connect(_clicker, &Clicker::mouseButtonChanged, this, [this](Qt::MouseButton mouseButton) {
        int index = 0;

        switch (mouseButton) {
        case Qt::MouseButton::LeftButton: index = 0; break;
        case Qt::MouseButton::RightButton: index = 1; break;
        case Qt::MouseButton::MiddleButton: index = 2; break;
        default: break;
        }

        ui->comboBoxMouseButton->setCurrentIndex(index);
        if (_isLoadingConfig) _configManager.write(*_clicker);
    });

    connect(_clicker, &Clicker::clicksPerSecondChanged, this, [this](unsigned clicks) {
        ui->spinBoxClicksPerSecond->setValue(clicks);
        if (_isLoadingConfig) _configManager.write(*_clicker);
    });

    connect(_clicker, &Clicker::stateChanged, this, [this](Clicker::ClickerState state) {
        StyleManager::setStyleState(ui->labelClickerStateValue, state);
        switch(state) {
        case Clicker::ClickerState::BindingKey: _setAppIcon(QIcon(":/sources/iconDefault.ico")); break;
        case Clicker::ClickerState::Ready: _setAppIcon(QIcon(":/sources/iconDefault.ico")); break;
        case Clicker::ClickerState::Active: _setAppIcon(QIcon(":/sources/iconActive.ico")); break;
        }
    });

    connect(_clicker, &Clicker::warningChanged, this, [this](Clicker::Warning warning) {
        StyleManager::setStyleWarning(ui->labelWarningValue, warning);
    });

    connect(_clicker, &Clicker::autoRunChanged, this, [this](bool arg1) {
        ui->checkBoxAutostartWithWindows->setCheckState(arg1 ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        if (_isLoadingConfig) _configManager.write(*_clicker);
    });

    connect(_clicker, &Clicker::runInTrayChanged, this, [this](bool arg1) {
        ui->checkBoxRunInSystemTray->setCheckState(arg1 ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        if (_isLoadingConfig) _configManager.write(*_clicker);
    });

    _clicker->loadConfig(_configManager);
    _isLoadingConfig = true;
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::closeEvent(QCloseEvent *event) {
    if (QApplication::activeModalWidget()) {
        // Пока открыт модальный диалог (например, KeyBindDialog),
        // не даём закрыть MainWindow — иначе quit() уйдёт во вложенный
        // event loop диалога, а не во внешний, и приложение зависнет.
        event->ignore();
        return;
    }
    if (_trayManager && _trayManager->isVisible()) {
        this->hide();
        event->ignore();
    }
    else event->accept();
}


void MainWindow::on_comboBoxActivationMode_currentIndexChanged(int index) {
    _clicker->setMode(static_cast<Clicker::ActivationMode>(index));
}

void MainWindow::on_pushButtonActivationKey_clicked() {
    KeyBindDialog dialog(_clicker->getMouseButton(), this);

    dialog.adjustSize(); // Обновляем размеры окна, чтобы расчеты были точными
    QPoint parentCenter = this->geometry().center();
    dialog.move(parentCenter.x() - dialog.width() / 2, parentCenter.y() - dialog.height() / 1.6);

    dialog.exec();

    if (dialog.isSelected()) {
        _clicker->setBind(dialog.getBind());
        ui->pushButtonActivationKey->setText(dialog.getBind().toString());

        // хук вызывается из двух мест, по-хорошему это потом надо переписать
        _clicker->startHook();
    }
}

void MainWindow::on_comboBoxMouseButton_currentIndexChanged(int index) {
    Qt::MouseButton mouseButton = Qt::MouseButton::NoButton;
    switch (index) {
    case 0: mouseButton = Qt::MouseButton::LeftButton; break;
    case 1: mouseButton = Qt::MouseButton::RightButton; break;
    case 2: mouseButton = Qt::MouseButton::MiddleButton; break;
    default: break;
    }

    if (mouseButton == _clicker->getBind().getMouseButton()) {
        QSignalBlocker blocker(ui->comboBoxMouseButton);
        ui->comboBoxMouseButton->setCurrentIndex(_lastValidMouseButtonIndex);

        QMessageBox::warning(this, "QClicker",
            "Can't set click button same as activation bind.");

        return;
    }

    _lastValidMouseButtonIndex = index;
    _clicker->setMouseButton(mouseButton);
}

void MainWindow::on_spinBoxClicksPerSecond_valueChanged(int arg1) {
    _clicker->setClicksPerSecond(arg1);

    if (arg1 > 0)
        _clicker->setWarning(Clicker::Warning::None);
    else
        _clicker->setWarning(Clicker::Warning::ZeroCps);
}
void MainWindow::on_spinBoxClicksPerSecond_editingFinished() { ui->spinBoxClicksPerSecond->clearFocus(); }

void MainWindow::on_pushButtonResetSettings_clicked() {
    _clicker->setMode(Clicker::ActivationMode::Hold);
    _clicker->setBind(InputBinding());
    _clicker->setMouseButton(Qt::LeftButton);
    _clicker->setClicksPerSecond(0);
}

void MainWindow::on_pushButtonExit_clicked() { qApp->quit(); }

void MainWindow::on_pushButtonOk_clicked() { this->showMinimized(); }

void MainWindow::on_checkBoxAutostartWithWindows_checkStateChanged(const Qt::CheckState &arg1) {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QString appName = "QClicker";

    if (arg1 == Qt::CheckState::Checked) {
        _clicker->setAutoRun(true);
        QString appPath = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
        settings.setValue(appName, "\"" + appPath + "\"");
    }
    else {
        settings.remove(appName);
        _clicker->setAutoRun(false);
    }
}

void MainWindow::on_checkBoxRunInSystemTray_checkStateChanged(const Qt::CheckState &arg1) {
    if (arg1 == Qt::CheckState::Checked) {
        _clicker->setRunInTray(true);
        _trayManager = new TrayManager(this, this);
        connect(this, &MainWindow::iconChanged, _trayManager, &TrayManager::setTrayIcon);
    }

    else {
        if (_trayManager) {
            _clicker->setRunInTray(false);
            delete _trayManager;
            _trayManager = nullptr;
        }
    }
}

void MainWindow::_setAppIcon(const QIcon &icon) {
    setWindowIcon(icon);                  // Меняем у главного окна
    emit iconChanged(icon);    // Рассылаем сигнал всем заинтересованным
}
