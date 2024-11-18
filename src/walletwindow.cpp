

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

    passwordDock = std::make_unique<QDockWidget>();
    passwordDock->setAllowedAreas(Qt::TopDockWidgetArea);
    passwordDock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    std::unique_ptr<QWidget> dockContainer = std::make_unique<QWidget>(passwordDock.get());
    dockContainer->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    std::unique_ptr<QHBoxLayout> passwordLayout = std::make_unique<QHBoxLayout>();

    std::unique_ptr<QLabel> label = std::make_unique<QLabel>(tr("Wallet password:"));
    label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordLayout->addWidget(label.release());

    passwordEdit = std::make_unique<QLineEdit>(nullptr);
    passwordEdit->setEchoMode(QLineEdit::EchoMode::Password);
    passwordEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    passwordLayout->addWidget(passwordEdit.get());

    passwordBtn = std::make_unique<QPushButton>(tr("Unlock"));
    passwordBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    passwordLayout->addWidget(passwordBtn.get());

    dockContainer->setLayout(passwordLayout.release());

    passwordDock->setWidget(dockContainer.release());
    passwordDock->widget()->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

    addDockWidget(Qt::TopDockWidgetArea, passwordDock.get());

    connect(passwordBtn.get(), &QPushButton::pressed, this, &WalletWindow::requestUnlock);
}

WalletWindow::~WalletWindow() {}

void WalletWindow::requestWalletPassword() const
{
    if (passwordDock->isVisible()) throw std::runtime_error("Password is already requesting!");

    passwordEdit->clear();
    passwordDock->show();

}

void WalletWindow::requestUnlock() const
{
    passwordDock->hide();
}


