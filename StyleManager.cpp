#include "StyleManager.h"

#include "Clicker.h"

#include <QLabel>

void StyleManager::setStyleState(QLabel *labelState, Clicker::ClickerState state, bool isDark) {
    switch (state) {
    case Clicker::ClickerState::BindingKey:
        labelState->setText("Binding Key");
        labelState->setStyleSheet(isDark
          ? "QLabel {"
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
          : "QLabel {"
            "  color: #C22A1D;"
            "  background-color: #f0f0f0;"
            "  border: 1px solid #C22A1D;"
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
        labelState->setStyleSheet(isDark
          ? "QLabel {"
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
          : "QLabel {"
            "  color: #8E2DC5;"
            "  background-color: #f0f0f0;"
            "  border: 1px solid #8E2DC5;"
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
        labelState->setStyleSheet(isDark
          ? "QLabel {"
            "  color: #2ECC71;"
            "  background-color: #1a1a1a;"
            "  border: 1px solid #2ECC71;"
            "  border-radius: 4px;"
            "  padding: 4px 8px;"
            "  font-family: 'Consolas', 'Courier New', monospace;"
            "  font-weight: bold;"
            "  font-size: 11pt;"
            "  letter-spacing: 1px;"
            "}"
          : "QLabel {"
            "  color: #1E8449;"
            "  background-color: #f0f0f0;"
            "  border: 1px solid #1E8449;"
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

void StyleManager::setStyleWarning(QLabel *labelWarning, Clicker::Warning warning, bool isDark) {
    switch (warning) {
    case Clicker::Warning::None:
        labelWarning->setText("None");
        labelWarning->setStyleSheet(isDark
            ? "QLabel {"
              "  color: #2ECC71;"
              "  background-color: #1a1a1a;"
              "  border: 1px solid #2ECC71;"
              "  border-radius: 4px;"
              "  padding: 4px 8px;"
              "  font-family: 'Consolas', 'Courier New', monospace;"
              "  font-weight: bold;"
              "  font-size: 11pt;"
              "  letter-spacing: 1px;"
              "}"
            : "QLabel {"
              "  color: #1E8449;"
              "  background-color: #f0f0f0;"
              "  border: 1px solid #1E8449;"
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
        labelWarning->setStyleSheet(isDark
            ? "QLabel {"
              "  color: #FFC107;"
              "  background-color: #1a1a1a;"
              "  border: 1px solid #FFC107;"
              "  border-radius: 4px;"
              "  padding: 4px 8px;"
              "  font-family: 'Consolas', 'Courier New', monospace;"
              "  font-weight: bold;"
              "  font-size: 11pt;"
              "  letter-spacing: 1px;"
              "}"
            : "QLabel {"
              "  color: #B8860B;"
              "  background-color: #f0f0f0;"
              "  border: 1px solid #B8860B;"
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