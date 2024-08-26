#ifndef WALLETWINDOW_H
#define WALLETWINDOW_H

#include <memory>

#include <QMainWindow>
#include "newmnemonicdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class WalletWindow;
}
QT_END_NAMESPACE

class WalletWindow : public QMainWindow
{
    Q_OBJECT

public:
    WalletWindow(QWidget *parent = nullptr);
    ~WalletWindow();

private:
    std::unique_ptr<Ui::WalletWindow> ui;
    std::unique_ptr<NewMnemonicDialog> newMnemonicDlg;

    std::unique_ptr<QMenu> walletMenu;

    std::unique_ptr<QAction> addMnemonic;

    void AddMemonicDialog();
};
#endif // WALLETWINDOW_H
