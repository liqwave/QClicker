#include "KeyBindDialog.h"
#include "ui_KeyBindDialog.h"

#include <QTimer>

void KeyBindDialog::keyPressEvent(QKeyEvent *event) {
    if (!_isSelected)
    {
        _isSelected = true;
        _binding = InputBinding(static_cast<Qt::Key>(event->key()), event->nativeScanCode());
        ui->label->setText("Activation Key: " + _binding.toString());
        ui->labelImage->setPixmap(QPixmap(":/sources/iconActive.ico"));
        QTimer::singleShot(1000, this, &QDialog::accept);
    }
}

void KeyBindDialog::mousePressEvent(QMouseEvent *event) {
    if (!_isSelected) {
        if (event->button() == _clickerButton) {
            _hasErrorMessage = true;
            ui->label->setText("Can't bind: clicker is using this button");
            ui->label->setStyleSheet("color: #EB3324;");

            QTimer::singleShot(2000, this, [this]() {
                if (_hasErrorMessage) {
                ui->label->setText(_defaultMessageLabel);
                ui->label->setStyleSheet("color: palette(window-text);");
                }
            });
            return;
        }
        _isSelected = true;
        _hasErrorMessage = false;
        _binding = InputBinding(event->button());
        ui->labelImage->setPixmap(QPixmap(":/sources/iconActive.ico"));
        ui->label->setText("Activation Key: " + _binding.toString());
        QTimer::singleShot(1000, this, &QDialog::accept);
    }
}

KeyBindDialog::KeyBindDialog(Qt::MouseButton clickerButton, QWidget *parent)
    : QDialog(parent), ui(new Ui::KeyBindDialog), _clickerButton(clickerButton) {
    ui->setupUi(this);

    ui->labelImage->setPixmap(
        _pixmap.scaled(
            256,
            256,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );

    ui->labelImage->setAlignment(Qt::AlignCenter);
}

KeyBindDialog::~KeyBindDialog() {
    delete ui;
}
