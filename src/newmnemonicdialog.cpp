#include "newmnemonicdialog.h"
#include "ui_newmnemonicdialog.h"

#include "random.h"

#include <ranges>

NewMnemonicDialog::NewMnemonicDialog(const l15::stringvector& wordlist, QWidget *parent)
    : QDialog(parent), ui(std::make_unique<Ui::newMnemonicDialog>()), mnemonicParser(wordlist)
{
    ui->setupUi(this);

    ui->wordCountSelector->addItem(tr("12 words"));
    ui->wordCountSelector->addItem(tr("15 words"));
    ui->wordCountSelector->addItem(tr("18 words"));
    ui->wordCountSelector->addItem(tr("21 words"));
    ui->wordCountSelector->addItem(tr("24 words"));

    connect(ui->passWordCheck, &QAbstractButton::toggled, this, &NewMnemonicDialog::TogglePassword);
    connect(ui->wordCountSelector, &QComboBox::currentIndexChanged, this, &NewMnemonicDialog::ResetWordForm);
    connect(ui->clearBtn, &QPushButton::clicked, this, &NewMnemonicDialog::ResetWordForm);
    connect(ui->generateBtn, &QPushButton::clicked, this, &NewMnemonicDialog::GeneratePhrase);
    connect(ui->passWordEdit, &QLineEdit::textChanged, this, &NewMnemonicDialog::MnemonicChanged);

}

NewMnemonicDialog::~NewMnemonicDialog()
{
}

bool NewMnemonicDialog::CheckInput() const
{
    bool res = ui->wordCountSelector->currentIndex() != -1;
    if (res) {
        for (const auto& e: wordEdits)
            res &= !e->displayText().isEmpty();
    }
    res &= ui->passWordCheck->isChecked() ^ ui->passWordEdit->displayText().isEmpty();

    ui->okBtn->setEnabled(res);

    return res;
}

void NewMnemonicDialog::open()
{
    ResetState();
    QDialog::open();
}

void NewMnemonicDialog::reject()
{
    ResetState();
    QDialog::reject();
}

void NewMnemonicDialog::ResetState()
{
    ui->wordCountSelector->setCurrentIndex(-1);
    ui->passWordCheck->setChecked(true);
    ResetWordForm();
}

void NewMnemonicDialog::TogglePassword(bool enable)
{
    ui->passWordEdit->clear();
    ui->passWordEdit->setEnabled(enable);
    CheckInput();
}

void NewMnemonicDialog::ResetWordForm()
{
    ui->okBtn->setEnabled(false);
    ui->passWordEdit->clear();
    for (auto& edit: wordEdits) {
        disconnect(edit.get());
        edit->clear();
        ui->wordGrid->removeWidget(edit.get());
    }
    wordEdits.clear();

    int index = ui->wordCountSelector->currentIndex();
    if (index == -1) {
        ui->wordsLabel->setText(tr("Please, select the mnemonic phrase length above"));
        ui->clearBtn->hide();
        ui->generateBtn->hide();
    }
    else {
        ui->wordsLabel->setText(tr("Fill your mnemonic phrase:"));
        ui->clearBtn->show();
        ui->generateBtn->show();
    }

    switch (index) {
    case MLEN_12:
        wordEdits.resize(12); break;
    case MLEN_15:
        wordEdits.resize(15); break;
    case MLEN_18:
        wordEdits.resize(18); break;
    case MLEN_21:
        wordEdits.resize(21); break;
    case MLEN_24:
        wordEdits.resize(24); break;
    default:
        wordEdits.clear(); break;
    }

    size_t i = 0;
    size_t rows = wordEdits.size() / 3;
    for(auto& edit: wordEdits) {
        QString label = tr("Word ") + QString::number(i+1);
        edit = std::make_unique<QLineEdit>(this);
        edit->setPlaceholderText(label);

        ui->wordGrid->addWidget(edit.get(), i%rows, i/rows);

        connect(edit.get(), &QLineEdit::textEdited, this, &NewMnemonicDialog::MnemonicChanged);

        ++i;
    }
}

void NewMnemonicDialog::GeneratePhrase()
{
    l15::sensitive_bytevector entropy;

    switch(ui->wordCountSelector->currentIndex()) {
    case MLEN_12:
        entropy.resize(16); break;
    case MLEN_15:
        entropy.resize(20); break;
    case MLEN_18:
        entropy.resize(24); break;
    case MLEN_21:
        entropy.resize(28); break;
    case MLEN_24:
        entropy.resize(32); break;
    default:
        throw std::runtime_error("Wrong mnemonic phrase size");

    }
    GetStrongRandBytes(entropy);

    for (auto mnemonic = mnemonicParser.EncodeEntropy(entropy); auto w: std::views::zip(mnemonic, wordEdits)) {
        w.second->setText(QString::fromUtf8(w.first.c_str(), w.first.length()));
    }

    CheckInput();
}

void NewMnemonicDialog::MnemonicChanged()
{
    CheckInput();
}
