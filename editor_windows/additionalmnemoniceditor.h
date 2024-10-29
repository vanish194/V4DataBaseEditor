#ifndef ADDITIONALMNEMONICEDITOR_H
#define ADDITIONALMNEMONICEDITOR_H

#include <QDialog>
#include "ui_additionalmnemoniceditor.h"
#include "storage.h"

class AdditionalMnemonicEditor : public QDialog
{
    Q_OBJECT

public:
    explicit AdditionalMnemonicEditor(QWidget *parent = nullptr);
    ~AdditionalMnemonicEditor();

    void setAdditionalMnemonic(const AdditionalMnemonic &mnemonic);
    AdditionalMnemonic getAdditionalMnemonic() const;

private slots:
    void onSaveButtonClicked();

private:
    Ui::AdditionalMnemonicEditor ui;

    AdditionalMnemonic currentAdditionalMnemonic;

    void loadCompanies();
    void loadMainMnemonics();
    void loadUnits();
};

#endif // ADDITIONALMNEMONICEDITOR_H
