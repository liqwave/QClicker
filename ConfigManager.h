#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "InputBinding.h"
#include "ClickerEnums.h"
#include <QString>

class Clicker;
class QJsonObject;

class ConfigManager {
private:
    QString _path = "QClickerConfig.json";

    ActivationMode _mode = ActivationMode::Hold;
    InputBinding _bind;
    Qt::MouseButton _mouseButton = Qt::MouseButton::LeftButton;
    unsigned _clicksPerSecond = 0;

    bool _autoRun = false;
    bool _runInTray = false;

    QJsonObject _bindToJson(const InputBinding& binding);
    InputBinding _jsonToBind(const QJsonObject& jsonObj);

public:
    ConfigManager();

    void write(const Clicker& clicker);

    ActivationMode getMode() const { return _mode; }
    InputBinding getBind() const { return _bind; }
    Qt::MouseButton getMouseButton() const { return _mouseButton; }
    unsigned getClicksPerSecond() const { return _clicksPerSecond; }
    bool getAutoRun() const { return _autoRun; }
    bool getRunInTray() const { return _runInTray; }
};

#endif // CONFIGMANAGER_H
