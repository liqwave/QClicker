#include "GlobalHook.h"

GlobalHook::GlobalHook() { _instance = this; }

GlobalHook::~GlobalHook() {
    stop();
}

void GlobalHook::start() {
    _keyboardHook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        keyboardProc,
        nullptr,
        0
        );

    _mouseHook = SetWindowsHookEx(
        WH_MOUSE_LL,
        mouseProc,
        nullptr,
        0
        );
}

void GlobalHook::stop() {
    if (_keyboardHook) {
        UnhookWindowsHookEx(_keyboardHook);
        _keyboardHook = nullptr;
    }

    if (_mouseHook) {
        UnhookWindowsHookEx(_mouseHook);
        _mouseHook = nullptr;
    }
}

void GlobalHook::setCallback(BindingCallback callback) {
    _callback = callback;
}

void GlobalHook::sendEvent(InputBinding binding, EventType type) {
    if (_instance && _instance->_callback) {
        _instance->_callback(binding, type);
    }
}

// ==========================
// Keyboard
// ==========================

LRESULT CALLBACK GlobalHook::keyboardProc(
    int code,
    WPARAM wParam,
    LPARAM lParam
    ) {
    if (code == HC_ACTION) {
        auto* info = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        // Игнорируем события, сгенерированные программой (SendInput)
        if (info->flags & LLKHF_INJECTED) {
            return CallNextHookEx(nullptr, code, wParam, lParam);
        }

        InputBinding binding(
            static_cast<Qt::Key>(info->vkCode),
            info->scanCode
            );

        switch (wParam) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            sendEvent(binding, EventType::Pressed);
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            sendEvent(binding, EventType::Released);
            break;
        }
    }

    return CallNextHookEx(
        nullptr,
        code,
        wParam,
        lParam
        );
}

// ==========================
// Mouse
// ==========================

LRESULT CALLBACK GlobalHook::mouseProc(
    int code,
    WPARAM wParam,
    LPARAM lParam
    ) {
    if (code == HC_ACTION) {
        auto* info = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

        // Игнорируем события, сгенерированные программой (SendInput)
        if (info->flags & LLMHF_INJECTED) {
            return CallNextHookEx(nullptr, code, wParam, lParam);
        }

        switch (wParam) {
        case WM_LBUTTONDOWN:
            sendEvent(
                InputBinding(Qt::LeftButton),
                EventType::Pressed
                );
            break;

        case WM_LBUTTONUP:
            sendEvent(
                InputBinding(Qt::LeftButton),
                EventType::Released
                );
            break;

        case WM_RBUTTONDOWN:
            sendEvent(
                InputBinding(Qt::RightButton),
                EventType::Pressed
                );
            break;

        case WM_RBUTTONUP:
            sendEvent(
                InputBinding(Qt::RightButton),
                EventType::Released
                );
            break;

        case WM_MBUTTONDOWN:
            sendEvent(
                InputBinding(Qt::MiddleButton),
                EventType::Pressed
                );
            break;

        case WM_MBUTTONUP:
            sendEvent(
                InputBinding(Qt::MiddleButton),
                EventType::Released
                );
            break;

        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP: {
            WORD button = HIWORD(info->mouseData);

            InputBinding binding =
                (button == XBUTTON1)
                    ? InputBinding(Qt::BackButton)
                    : InputBinding(Qt::ForwardButton);

            sendEvent(
                binding,
                wParam == WM_XBUTTONDOWN
                    ? EventType::Pressed
                    : EventType::Released
            );

            break;
            }
        }
    }

    return CallNextHookEx(
        nullptr,
        code,
        wParam,
        lParam
    );
}