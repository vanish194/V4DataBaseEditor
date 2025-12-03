#ifndef MAINMNEMONICADDWINDOW_H
#define MAINMNEMONICADDWINDOW_H

#include <QDialog>
#include "ui_mainmnemonicaddwindow.h"
#include "storage.h"

class MainMnemonicAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainMnemonicAddWindow(QWidget *parent = nullptr);
    ~MainMnemonicAddWindow();

    void setNewMainMnemonicId(int id);

    MainMnemonic getNewMainMnemonic() const;

private slots:
    void onAddButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::MainMnemonicAddWindow ui;

    MainMnemonic newMainMnemonic;

    void loadUnits();
};

#endif // MAINMNEMONICADDWINDOW_H
