#ifndef GLOBALHOOK_H
#define GLOBALHOOK_H

#include <Windows.h>
#include <functional>

#include "InputBinding.h"

class GlobalHook {
public:
    enum class EventType { Pressed, Released };

    using BindingCallback = std::function<void(InputBinding, EventType)>;

private:
    HHOOK _keyboardHook = nullptr;
    HHOOK _mouseHook = nullptr;

    BindingCallback _callback;

    inline static GlobalHook* _instance = nullptr;

    static LRESULT CALLBACK keyboardProc(
        int code,
        WPARAM wParam,
        LPARAM lParam
        );

    static LRESULT CALLBACK mouseProc(
        int code,
        WPARAM wParam,
        LPARAM lParam
        );

    static void sendEvent(
        InputBinding binding,
        EventType type
        );

public:
    GlobalHook();
    ~GlobalHook();

    void start();
    void stop();

    bool isStarted() const noexcept {
        return _keyboardHook != nullptr && _mouseHook != nullptr;
    }

    void setCallback(BindingCallback callback);
};


#endif // GLOBALHOOK_H
