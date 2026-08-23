#include "ConfigManager.h"
#include "Clicker.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

QJsonObject ConfigManager::_bindToJson(const InputBinding& binding) {
    QJsonObject bindObj;
    InputBinding::Type typeBind = binding.getType();

    switch(typeBind) {
    case InputBinding::Type::None:
        bindObj["Type"] = static_cast<int>(typeBind);
        break;
    case InputBinding::Type::Keyboard:
        bindObj["Type"] = static_cast<int>(typeBind);
        bindObj["Key"] = static_cast<int>(binding.getKey());
        bindObj["NativeCode"] = static_cast<int>(binding.getNativeCode());
        break;
    case InputBinding::Type::MouseButton:
        bindObj["Type"] = static_cast<int>(typeBind);
        bindObj["MouseButton"] = static_cast<int>(binding.getMouseButton());
        break;
    }
    return bindObj;
}

InputBinding ConfigManager::_jsonToBind(const QJsonObject &bindObj) {
    InputBinding::Type jsonObjType = static_cast<InputBinding::Type>(bindObj["Type"].toInt());

    switch(jsonObjType) {
    case InputBinding::Type::None:
        return InputBinding();
    case InputBinding::Type::Keyboard:
        return InputBinding(static_cast<Qt::Key>(bindObj["Key"].toInt()), static_cast<quint32>(bindObj["NativeCode"].toInteger()));
    case InputBinding::Type::MouseButton:
        return InputBinding(static_cast<Qt::MouseButton>(bindObj["MouseButton"].toInt()));
    }
    return InputBinding();
}

ConfigManager::ConfigManager() {
    if (QFile::exists(_path)) {
        QFile file(_path);

        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();

            QJsonDocument document = QJsonDocument::fromJson(data);

            if (document.isNull()) return;

            QJsonObject jsonObj = document.object();

            _mode = static_cast<Clicker::ActivationMode>(jsonObj["ActivationMode"].toInt());
            _bind = _jsonToBind(jsonObj["ActivationKey"].toObject());
            _mouseButton = static_cast<Qt::MouseButton>(jsonObj["MouseButton"].toInt());
            _clicksPerSecond = jsonObj["ClicksPerSecond"].toInt();
            _clickDutyCycle = jsonObj["ClickDutyCycle"].toDouble();
            _theme = static_cast<Clicker::Theme>(jsonObj["Theme"].toInt());
            _autoRun = jsonObj["AutostartWithWindows"].toBool();
            _runInTray = jsonObj["RunInSystemTray"].toBool();
        }
    }
}

void ConfigManager::write(const Clicker &clicker) {
    QJsonObject jsonObj;

    jsonObj["ActivationMode"] = static_cast<int>(clicker.getMode());
    jsonObj["ActivationKey"] = _bindToJson(clicker.getBind());
    jsonObj["MouseButton"] = static_cast<int>(clicker.getMouseButton());
    jsonObj["ClicksPerSecond"] = static_cast<int>(clicker.getClicksPerSecond());
    jsonObj["ClickDutyCycle"] = static_cast<double>(clicker.getClickDutyCycle());
    jsonObj["Theme"] = static_cast<int>(clicker.getTheme());
    jsonObj["AutostartWithWindows"] = clicker.getAutoRun();
    jsonObj["RunInSystemTray"] = clicker.getRunInTray();

    QJsonDocument document(jsonObj);

    QFile file(_path);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(document.toJson());
        file.close();
    }

}
