#ifndef KEYBINDDIALOG_H
#define KEYBINDDIALOG_H

#include <QDialog>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPixmap>
#include "InputBinding.h"

namespace Ui { class KeyBindDialog; }

class KeyBindDialog : public QDialog {
    Q_OBJECT

private:
    Ui::KeyBindDialog *ui = nullptr;

    Qt::MouseButton _clickerButton;
    InputBinding _binding;
    bool _isSelected = false;
    bool _hasErrorMessage = false;

    QString _defaultMessageLabel = "Press any button";

    QPixmap _pixmap = QPixmap(":/sources/iconDefault.ico");

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit KeyBindDialog(Qt::MouseButton clickerButton, QWidget *parent = nullptr);
    ~KeyBindDialog();

    InputBinding getBind() const { return _binding; }
    bool isSelected() const { return _isSelected; }
};

#endif // KEYBINDDIALOG_H
