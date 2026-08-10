#ifndef INPUTBINDING_H
#define INPUTBINDING_H

#include <Qt>
#include <QString>
#include <QKeySequence>

class InputBinding {
public:
    enum class Type { None, Keyboard, MouseButton };

private:
    Type _type = Type::None;
    Qt::Key _key = Qt::Key_unknown;
    quint32 _nativeCode = 0;
    Qt::MouseButton _mouseButton = Qt::NoButton;

public:
    InputBinding() = default;
    explicit InputBinding(Qt::Key key, quint32 nativeCode)
        : _type(Type::Keyboard), _key(key), _nativeCode(nativeCode) { }
    explicit InputBinding(Qt::MouseButton button) : _type(Type::MouseButton), _mouseButton(button) { }

    bool operator==(const InputBinding& other) const noexcept {
        if (_type != other._type) return false;

        switch (_type) {
        case Type::Keyboard:
            return _nativeCode == other._nativeCode;

        case Type::MouseButton:
            return _mouseButton == other._mouseButton;

        case Type::None:
            return false;
        }

        return false;
    }


    QString toString() const {
        if  (!isValid()) return "SELECT";

        if (isKeyboard()) return QKeySequence(_key).toString();

        switch (_mouseButton) {
        case Qt::LeftButton:
            return "Left";

        case Qt::RightButton:
            return "Right";

        case Qt::MiddleButton:
            return "Middle";

        case Qt::BackButton:
            return "XButton 1";

        case Qt::ForwardButton:
            return "XButton 2";

        default:
            return "Untitled";
        }
    }

    Type getType() const { return _type; }
    bool isValid() const { return _type != Type::None; }
    bool isKeyboard() const { return _type == Type::Keyboard; }
    bool isMouseButton() const { return _type == Type::MouseButton; }

    Qt::Key getKey() const { return _key; }
    quint32 getNativeCode() const { return _nativeCode; }
    Qt::MouseButton getMouseButton() const { return _mouseButton; }
};

#endif // INPUTBINDING_H
