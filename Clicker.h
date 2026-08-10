#ifndef CLICKER_H
#define CLICKER_H

#include "InputBinding.h"
#include "GlobalHook.h"
#include "ClickerEnums.h"

#include <QObject>
#include <Qt>
#include <windows.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class ConfigManager;

class Clicker : public QObject {
    Q_OBJECT

public:
    using ActivationMode = ::ActivationMode;
    using ClickerState   = ::ClickerState;
    using Warning        = ::Warning;

private:
    GlobalHook _hook;

    // Рабочий поток
    std::thread _workerThread;
    std::atomic<bool> _isRunning = false;
    std::atomic<bool> _stopWorker = false;
    std::condition_variable _cv;
    std::mutex _cvMutex;

    ActivationMode _mode = ActivationMode::Hold;
    InputBinding _bind;
    Qt::MouseButton _mouseButton = Qt::MouseButton::LeftButton;
    std::atomic<unsigned> _clicksPerSecond = 0;

    std::atomic<ClickerState> _state = ClickerState::BindingKey;
    std::atomic<Warning> _warning = Warning::ZeroCps;

    bool _autoRun = false;
    bool _runInTray = false;

    void _workerLoop();
    void _performSingleClick();
    void _sendMouseEvent(bool isDown);
    void _preciseSleep(double milliseconds);

    void _startClicking();
    void _stopClicking();

public:
    explicit Clicker(QObject* parent = nullptr);
    ~Clicker();

    void loadConfig(const ConfigManager& config);

    void startHook();

    bool isRunning() const { return _isRunning.load(); }

    ActivationMode getMode() const { return _mode; }
    InputBinding getBind() const { return _bind; }
    Qt::MouseButton getMouseButton() const { return _mouseButton; }
    unsigned getClicksPerSecond() const { return _clicksPerSecond.load(); }
    ClickerState getState() const { return _state; }
    Warning getWarning() const { return _warning; }
    bool getAutoRun() const { return _autoRun; }
    bool getRunInTray() const { return _runInTray; }

    void setMode(ActivationMode mode) { _mode = mode; emit modeChanged(mode); }
    void setBind(InputBinding bind) {
        if (bind.isValid()) setState(ClickerState::Ready);
        else setState(ClickerState::BindingKey);
        _bind = bind;
        emit bindChanged(bind);
    }
    void setMouseButton(Qt::MouseButton mouseButton) { _mouseButton = mouseButton; emit mouseButtonChanged(mouseButton); }
    void setClicksPerSecond(unsigned clicks) { _clicksPerSecond = clicks; emit clicksPerSecondChanged(clicks); }
    void setState(ClickerState state) { _state = state; emit stateChanged(state); }
    void setWarning(Warning warning) { _warning = warning; emit warningChanged(warning); }
    void setAutoRun(bool value) { _autoRun = value; emit autoRunChanged(value); }
    void setRunInTray(bool value) { _runInTray = value; emit runInTrayChanged(value); }

signals:
    void modeChanged(ActivationMode mode);
    void bindChanged(InputBinding bind);
    void mouseButtonChanged(Qt::MouseButton mouseButton);
    void clicksPerSecondChanged(unsigned clicks);
    void stateChanged(ClickerState state);
    void warningChanged(Warning warning);
    void autoRunChanged(bool value);
    void runInTrayChanged(bool value);
};

#endif // CLICKER_H