#ifndef WALLETWINDOW_H
#define WALLETWINDOW_H

#include <memory>

#include <QMainWindow>
#include "newmnemonicdialog.h"

#include "config.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class WalletWindow;
}
QT_END_NAMESPACE

class WalletWindow : public QMainWindow
{
    Q_OBJECT

    std::shared_ptr<bscratcher::Config> mConfig;

    std::unique_ptr<Ui::WalletWindow> ui;
    std::unique_ptr<NewMnemonicDialog> newMnemonicDlg;

    std::unique_ptr<QMenu> walletMenu;
    std::unique_ptr<QAction> addMnemonic;

    std::unique_ptr<QDockWidget> passwordDock;

public:
    WalletWindow(std::shared_ptr<bscratcher::Config> config, QWidget *parent = nullptr);
    ~WalletWindow() override;

private:
    void RequestWalletPassword();
};
#endif // WALLETWINDOW_H
