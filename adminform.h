#ifndef ADMINFORM_H
#define ADMINFORM_H

#include <QWidget>
#include <QDialog>
#include "db.h"
#include <QStandardItemModel>
#include <QStandardItem>

namespace Ui {
class AdminForm;
}

class AdminForm : public QDialog
{
    Q_OBJECT

public:
    explicit AdminForm(QWidget *parent = 0);
    void ViewUser();
    ~AdminForm();

private slots:
    void on_AddUser_clicked();

    void on_DeleteUser_clicked();

private:
    Ui::AdminForm *ui;
};

#endif // ADMINFORM_H
