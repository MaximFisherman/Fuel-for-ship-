#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "dialog_add_engine.h"

namespace Ui {
class AddShip;
}

class AddShip: public QDialog
{
    Q_OBJECT

public:
    explicit AddShip(QWidget *parent = 0);
    void CheckInstertParam();
    void AddEngine();
    void EnabledButtonRemove();
    ~AddShip();

private slots:

    void on_AddEngine_clicked();

    void on_NameShip_textChanged(const QString &arg1);

    void on_Confirm_clicked();

    void on_Owner_textChanged(const QString &arg1);

    void on_Flag_textChanged(const QString &arg1);

    void on_Year_textChanged(const QString &arg1);

    void on_Height_textChanged(const QString &arg1);

    void on_Width_textChanged(const QString &arg1);

    void on_Length_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_RemoveEngine_clicked();

private:
    Ui::AddShip *ui;
};

#endif // DIALOG_H
