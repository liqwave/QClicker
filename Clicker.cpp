#include "Clicker.h"
#include "ConfigManager.h"

#include <windows.h>
#include <timeapi.h>
#include <chrono>
#include <QDebug>
#include <emmintrin.h>

#pragma comment(lib, "Winmm.lib")

Clicker::Clicker(QObject* parent) : QObject(parent) {
    // Включаем высокое разрешение системного таймера Windows (1 мс)
    timeBeginPeriod(1);

    // Запускаем фоновый поток один раз на все время жизни объекта
    _workerThread = std::thread([this]() { _workerLoop(); });
}

Clicker::~Clicker() {
    _hook.stop();

    _stopWorker = true;
    _cv.notify_all();

    if (_workerThread.joinable()) {
        _workerThread.join();
    }

    // Возвращаем стандартное разрешение таймера
    timeEndPeriod(1);
}

void Clicker::loadConfig(const ConfigManager& config) {
    setMode(config.getMode());
    setBind(config.getBind());
    setMouseButton(config.getMouseButton());
    setClicksPerSecond(config.getClicksPerSecond());
    setAutoRun(config.getAutoRun());
    setRunInTray(config.getRunInTray());

    // хук вызывается из двух мест, по-хорошему это потом надо переписать
    startHook();
}

void Clicker::startHook() {
    if (_hook.isStarted()) return;

    qDebug() << "startHook()";

    _hook.setCallback(
        [this](InputBinding binding, GlobalHook::EventType event) {
            if (binding == _bind) {
                if (_mode == ActivationMode::Hold) {
                    if (event == GlobalHook::EventType::Pressed) {
                        _startClicking();
                    }

                    if (event == GlobalHook::EventType::Released) {
                        _stopClicking();
                    }
                }

                if (_mode == ActivationMode::Toggle) {
                    if (event == GlobalHook::EventType::Pressed) {
                        _isRunning ? _stopClicking() : _startClicking();
                    }
                }
            }
        });

    _hook.start();
}

void Clicker::_startClicking() {
    if (_clicksPerSecond == 0) return;
    if (_isRunning) return;

    setState(ClickerState::Active);
    _isRunning = true;
    _cv.notify_one();
}

void Clicker::_stopClicking() {
    // Мгновенная неблокирующая остановка
    setState(ClickerState::Ready);
    _isRunning = false;
}

void Clicker::_workerLoop() {
    while (!_stopWorker) {
        std::unique_lock<std::mutex> lock(_cvMutex);
        _cv.wait(lock, [this]() { return _isRunning.load() || _stopWorker.load(); });

        if (_stopWorker) break;

        lock.unlock();

        while (_isRunning && !_stopWorker) {
            _performSingleClick();
        }
    }
}

void Clicker::_performSingleClick() {
    unsigned cps = _clicksPerSecond.load();
    if (cps == 0) return;

    double intervalMs = 1000.0 / cps;

    double pressDurationMs = intervalMs * 0.5;
    double releaseDurationMs = intervalMs - pressDurationMs;

    // 1. Нажатие (DOWN)
    _sendMouseEvent(true);
    _preciseSleep(pressDurationMs);

    // 2. Отпускание (UP) -- отправляем ГАРАНТИРОВАННО!
    // Начатый клик обязан завершиться, иначе кнопка останется зажатой в Windows.
    _sendMouseEvent(false);

    // 3. А вот паузу перед СЛЕДУЮЩИМ кликом выдерживаем только если мы все еще работаем
    if (_isRunning) {
        _preciseSleep(releaseDurationMs);
    }
}

void Clicker::_sendMouseEvent(bool isDown) {
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    switch (_mouseButton) {
    case Qt::LeftButton:
        input.mi.dwFlags = isDown ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
        break;

    case Qt::RightButton:
        input.mi.dwFlags = isDown ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
        break;

    case Qt::MiddleButton:
        input.mi.dwFlags = isDown ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
        break;

    default:
        return;
    }

    SendInput(1, &input, sizeof(INPUT));
}

void Clicker::_preciseSleep(double milliseconds) {
    auto start = std::chrono::steady_clock::now();
    auto target = start + std::chrono::duration<double, std::milli>(milliseconds);

    while (std::chrono::steady_clock::now() < target && _isRunning && !_stopWorker) {
        auto remaining = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(target - std::chrono::steady_clock::now()).count();

        // Спим системным потоком только если осталось БОЛЬШЕ 12 мс (квант ОС).
        // Все короче 12 мс крутим через быстрый spin-wait (_mm_pause).
        if (remaining > 12.0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(remaining - 2.0)));
        } else {
            _mm_pause(); // Аппаратная пауза процессора
        }
    }
}
