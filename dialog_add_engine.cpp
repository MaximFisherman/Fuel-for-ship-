#include "dialog_add_ship.h"
#include "dialog_add_engine.h"
#include "ui_dialog_add_engine.h"
#include "db.h"

dialog_add_engine::dialog_add_engine(QWidget *parent, QString nameShip) :
    QDialog(parent),
    ui(new Ui::dialog_add_engine)
{
    this->nameShip = nameShip;
    ui->setupUi(this);
    ui->acceptButton->setEnabled(false);
}

dialog_add_engine::~dialog_add_engine()
{
    delete ui;
}

void dialog_add_engine::CheckInsert(){
    if(ui->TypeEngine->text() != "" && ui->Power->text() != "" && ui->FuelConsumption->text() != "" && ui->Torque->text() != "" && ui->MarkaEngine->text() != "" ){
        ui->acceptButton->setEnabled(true);;
        qDebug("Testing check");
    }

    qDebug() <<nameShip;
}

void dialog_add_engine::on_TypeEngine_textChanged(const QString &arg1)
{
    CheckInsert();
}

void dialog_add_engine::on_Torque_textChanged(const QString &arg1)
{
    CheckInsert();
}

void dialog_add_engine::on_Power_textChanged(const QString &arg1)
{
    CheckInsert();
}

void dialog_add_engine::on_FuelConsumption_textChanged(const QString &arg1)
{
    CheckInsert();
}

void dialog_add_engine::on_lineEdit_textChanged(const QString &arg1)
{
    CheckInsert();
}

void dialog_add_engine::on_acceptButton_clicked()
{
    DbManager db(path);
    AddShip addShip;
    addShip.EnabledButtonRemove();
    qDebug() <<nameShip;

    db.addEngine(ui->TypeEngine->text(),ui->Torque->text(), ui->Power->text(), ui->FuelConsumption->text(), nameShip, ui->MarkaEngine->text());
    db.CloseConection();
}



void dialog_add_engine::on_MarkaEngine_textChanged(const QString &arg1)
{
    CheckInsert();
}

void dialog_add_engine::on_CancelButton_clicked()
{

}
