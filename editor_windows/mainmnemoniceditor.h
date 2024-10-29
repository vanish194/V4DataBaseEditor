#ifndef MAINMNEMONICEDITOR_H
#define MAINMNEMONICEDITOR_H

#include <QDialog>
#include "ui_mainmnemoniceditor.h"
#include "storage.h"

class MainMnemonicEditor : public QDialog
{
    Q_OBJECT

public:
    explicit MainMnemonicEditor(QWidget *parent = nullptr);
    ~MainMnemonicEditor();

    void setMainMnemonic(const MainMnemonic &mnemonic);
    MainMnemonic getMainMnemonic() const;

private slots:
    void onSaveButtonClicked();

private:
    Ui::MainMnemonicEditor ui;

    MainMnemonic currentMainMnemonic;

    void loadUnits();
};

#endif // MAINMNEMONICEDITOR_H
