#ifndef DIALOG_ADD_ENGINE_H
#define DIALOG_ADD_ENGINE_H

#include <QDialog>

namespace Ui {
class dialog_add_engine;
}

class dialog_add_engine : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_add_engine(QWidget *parent = 0, QString nameShip = 0);
    void CheckInsert();
    QString nameShip;
    ~dialog_add_engine();

private slots:
    void on_TypeEngine_textChanged(const QString &arg1);

    void on_Torque_textChanged(const QString &arg1);

    void on_Power_textChanged(const QString &arg1);

    void on_FuelConsumption_textChanged(const QString &arg1);

    void on_acceptButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_MarkaEngine_textChanged(const QString &arg1);

    void on_CancelButton_clicked();

private:
    Ui::dialog_add_engine *ui;
};

#endif // DIALOG_ADD_ENGINE_H
