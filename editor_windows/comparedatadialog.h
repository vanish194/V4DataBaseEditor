#ifndef COMPAREDATADIALOG_H
#define COMPAREDATADIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "storage.h"

namespace Ui {
class CompareDataDialog;
}

class CompareDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CompareDataDialog(QWidget *parent = nullptr);
    ~CompareDataDialog();

private slots:
    void onRevertSelected();

private:
    Ui::CompareDataDialog *ui;
    Storage *storage;
    QStandardItemModel *model;

    void populateData();
};

#endif // COMPAREDATADIALOG_H
