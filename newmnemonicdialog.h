#ifndef NEWMNEMONICDIALOG_H
#define NEWMNEMONICDIALOG_H

#include <memory>
#include <QDialog>
#include <QLineEdit>

#include "mnemonicwordlist.hpp"
#include "mnemonic.hpp"

namespace Ui {
class newMnemonicDialog;
}

enum MnemonicLength {MLEN_12, MLEN_15, MLEN_18, MLEN_21, MLEN_24};

class NewMnemonicDialog : public QDialog
{
    Q_OBJECT

    std::unique_ptr<Ui::newMnemonicDialog> ui;
    l15::core::MnemonicParser mnemonicParser;

    std::vector<std::unique_ptr<QLineEdit>> wordEdits;

public:
    explicit NewMnemonicDialog(std::vector<std::string> wordlist, QWidget *parent = nullptr);
    ~NewMnemonicDialog();

    bool CheckInput() const;

public Q_SLOTS:
    void open() override;
    void reject() override;

    void ResetState();
    void TogglePassword(bool enable);
    void ResetWordForm();

    void GeneratePhrase();

    void MnemonicChanged();
};

#endif // NEWMNEMONICDIALOG_H
