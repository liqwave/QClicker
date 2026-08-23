#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include "ClickerEnums.h"

class QLabel;

class StyleManager {
public:
    StyleManager() = default;

    static void setStyleState(QLabel* label, ClickerState state, bool isDark);
    static void setStyleWarning(QLabel* label, Warning warning, bool isDark);
};

#endif // STYLEMANAGER_H
