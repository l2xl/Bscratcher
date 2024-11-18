#ifndef WALLETWINDOW_H
#define WALLETWINDOW_H

#include <memory>

#include <QMainWindow>
#include "newmnemonicdialog.h"

#include "config.hpp"

namespace Ui {
class WalletWindow;
}

class WalletWindow : public QMainWindow
{
    Q_OBJECT

    std::shared_ptr<bscratcher::Config> mConfig;

    std::unique_ptr<Ui::WalletWindow> ui;
    std::unique_ptr<NewMnemonicDialog> newMnemonicDlg;

    std::unique_ptr<QMenu> walletMenu;
    std::unique_ptr<QAction> addMnemonic;

    std::unique_ptr<QDockWidget> passwordDock;
    std::unique_ptr<QLineEdit> passwordEdit;
    std::unique_ptr<QPushButton> passwordBtn;

public:
    WalletWindow(std::shared_ptr<bscratcher::Config> config, QWidget *parent = nullptr);
    ~WalletWindow() override;

private:
    void requestWalletPassword() const;

    void requestUnlock() const;
};
#endif // WALLETWINDOW_H
