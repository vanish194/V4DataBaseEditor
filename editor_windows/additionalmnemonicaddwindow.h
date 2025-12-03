#ifndef ADDITIONALMNEMONICADDWINDOW_H
#define ADDITIONALMNEMONICADDWINDOW_H

#include <QDialog>
#include "ui_additionalmnemonicaddwindow.h"
#include "storage.h"

class AdditionalMnemonicAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdditionalMnemonicAddWindow(QWidget *parent = nullptr);
    ~AdditionalMnemonicAddWindow();

    void setNewAdditionalMnemonicId(int id);

    AdditionalMnemonic getNewAdditionalMnemonic() const;

private slots:
    void onAddButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::AdditionalMnemonicAddWindow ui;

    AdditionalMnemonic newAdditionalMnemonic;

    void loadCompanies();
    void loadMainMnemonics();
    void loadUnits();
};

#endif // ADDITIONALMNEMONICADDWINDOW_H
