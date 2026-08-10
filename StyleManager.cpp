#include "StyleManager.h"

void StyleManager::setStyleState(QLabel *labelState, Clicker::ClickerState state) {
    switch (state) {
    case Clicker::ClickerState::BindingKey:
        labelState->setText("Binding Key");
        labelState->setStyleSheet(
            "QLabel {"
            "  color: #EB3324;"
            "  background-color: #1a1a1a;"
            "  border: 1px solid #EB3324;"
            "  border-radius: 4px;"
            "  padding: 4px 8px;"
            "  font-family: 'Consolas', 'Courier New', monospace;"
            "  font-weight: bold;"
            "  font-size: 11pt;"
            "  letter-spacing: 1px;"
            "}"
            );
        break;
    case Clicker::ClickerState::Ready:
        labelState->setText("Ready");
        labelState->setStyleSheet(
            "QLabel {"
            "  color: #D8A0DF;"
            "  background-color: #1a1a1a;"
            "  border: 1px solid #D8A0DF;"
            "  border-radius: 4px;"
            "  padding: 4px 8px;"
            "  font-family: 'Consolas', 'Courier New', monospace;"
            "  font-weight: bold;"
            "  font-size: 11pt;"
            "  letter-spacing: 1px;"
            "}"
        );
        break;
    case Clicker::ClickerState::Active:
        labelState->setText("Active");
        labelState->setStyleSheet("QLabel {"
            "  color: #2ECC71;" // зеленый
            "  background-color: #1a1a1a;"
            "  border: 1px solid #2ECC71;"
            "  border-radius: 4px;"
            "  padding: 4px 8px;"
            "  font-family: 'Consolas', 'Courier New', monospace;"
            "  font-weight: bold;"
            "  font-size: 11pt;"
            "  letter-spacing: 1px;"
            "}"
        );
        break;
    }
}

void StyleManager::setStyleWarning(QLabel *labelWarning, Clicker::Warning warning) {
    switch (warning) {
    case Clicker::Warning::None:
        labelWarning->setText("None");
        labelWarning->setStyleSheet(
            "QLabel {"
            "  color: #2ECC71;"           // зеленый
            "  background-color: #1a1a1a;"
            "  border: 1px solid #2ECC71;"
            "  border-radius: 4px;"
            "  padding: 4px 8px;"
            "  font-family: 'Consolas', 'Courier New', monospace;"
            "  font-weight: bold;"
            "  font-size: 11pt;"
            "  letter-spacing: 1px;"
            "}"
            );
        break;
    case Clicker::Warning::ZeroCps:
        labelWarning->setText("Zero CPS");
        labelWarning->setStyleSheet(
            "QLabel {"
            "  color: #FFC107;"           // жёлтый/янтарный
            "  background-color: #1a1a1a;"
            "  border: 1px solid #FFC107;"
            "  border-radius: 4px;"
            "  padding: 4px 8px;"
            "  font-family: 'Consolas', 'Courier New', monospace;"
            "  font-weight: bold;"
            "  font-size: 11pt;"
            "  letter-spacing: 1px;"
            "}"
        );
        break;
    }
}
