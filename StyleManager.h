#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QLabel>
#include "Clicker.h"

class StyleManager {
public:
    StyleManager() = default;

    static void setStyleState(QLabel* label, Clicker::ClickerState state);
    static void setStyleWarning(QLabel* label, Clicker::Warning warning);
};

#endif // STYLEMANAGER_H
