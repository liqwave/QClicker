#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include "ClickerEnums.h"

class QLabel;

class StyleManager {
public:
    StyleManager() = default;

    static void setStyleState(QLabel* label, ClickerState state);
    static void setStyleWarning(QLabel* label, Warning warning);
};

#endif // STYLEMANAGER_H
