

#include "walletwindow.h"
#include "ui_walletwindow.h"
#include "mnemonicwordlist.hpp"

#include <qpushbutton.h>

WalletWindow::WalletWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::WalletWindow>())
    , newMnemonicDlg(std::make_unique<NewMnemonicDialog>(mnemonic::en_word_list, this))
    , walletMenu()
    , addMnemonic(std::make_unique<QAction>(tr("&Add Mnemonic"), this))

{
    ui->setupUi(this);

    addMnemonic->setShortcut(QKeySequence::New);

    connect(addMnemonic.get(), SIGNAL(triggered()), newMnemonicDlg.get(), SLOT(open()));

    walletMenu.reset(menuBar()->addMenu(tr("Wallet")));
    walletMenu->addAction(addMnemonic.get());
}

WalletWindow::~WalletWindow() {}

void WalletWindow::AddMemonicDialog() {}
