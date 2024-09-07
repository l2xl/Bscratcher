

#include "walletwindow.h"
#include "walletwindow.h"
#include "ui_walletwindow.h"
#include "mnemonicwordlist.hpp"

#include <qpushbutton.h>
#include <qdockwidget.h>
#include <QLabel>
#include <qlayout.h>

WalletWindow::WalletWindow(std::shared_ptr<bscratcher::Config> conf, QWidget *parent)
    : QMainWindow(parent)
    , mConfig(std::move(conf))
    , ui(std::make_unique<Ui::WalletWindow>())
    , newMnemonicDlg(std::make_unique<NewMnemonicDialog>(mnemonic::en_word_list, this))
    , addMnemonic(std::make_unique<QAction>(tr("&Add Mnemonic"), this))

{
    ui->setupUi(this);

    addMnemonic->setShortcut(QKeySequence::New);

    connect(addMnemonic.get(), SIGNAL(triggered()), newMnemonicDlg.get(), SLOT(open()));

    walletMenu.reset(menuBar()->addMenu(tr("Wallet")));
    walletMenu->addAction(addMnemonic.get());

    RequestWalletPassword();
}

WalletWindow::~WalletWindow() {}

void WalletWindow::RequestWalletPassword()
{
    if (passwordDock) throw std::runtime_error("Password is already requesting!");

    passwordDock = std::make_unique<QDockWidget>();
    passwordDock->setAllowedAreas(Qt::TopDockWidgetArea);
    passwordDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    std::unique_ptr<QWidget> dockContainer = std::make_unique<QWidget>(passwordDock.get());
    std::unique_ptr<QHBoxLayout> passwordLayout = std::make_unique<QHBoxLayout>(dockContainer.get());

    passwordLayout->addWidget(new QLabel(tr("Wallet password:"), nullptr));

    std::unique_ptr<QLineEdit> passwordEdit = std::make_unique<QLineEdit>(nullptr);
    passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
    passwordLayout->addWidget(passwordEdit.release());

    passwordDock->setWidget(dockContainer.release());

    addDockWidget(Qt::TopDockWidgetArea, passwordDock.get());
}

