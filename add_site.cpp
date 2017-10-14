#include "add_site.h"
#include "ui_add_site.h"
#include "db.h"
#include <QDebug>
#include <cmath>

Add_site::Add_site(QWidget *parent, QString SpeedShip, QString NameShip, QString NameCourse) :
    QDialog(parent),
    ui(new Ui::Add_site)
{
    qDebug() <<"Speed ship check"<<SpeedShip;
    qDebug() <<"Name ship check"<<NameShip;
    this->NameShip = NameShip;
    this->SpeedShip = SpeedShip;
    this->NameCourse = NameCourse;
    ui->setupUi(this);
}

void Add_site::CheckInstertParam()
{
    if(ui->SpeedWind->text().isEmpty() || ui->BettaAngle->text().isEmpty() || ui->TimeSite->text().isEmpty())
    {
       ui->AcceptedButton->setEnabled(false);
    }else{
       ui->AcceptedButton->setEnabled(true);
    }
}

Add_site::~Add_site()
{
    delete ui;
}


void Add_site::on_AcceptedButton_clicked()
{
    DbManager db(path);
    int BeaufortScale;
    int TimeSite = ui->TimeSite->text().toInt();
    int WindCoefficient = 0; //4% down effective ship when with wind head
    double BettaAngle = ui->BettaAngle->text().toDouble();
    double WindSpeed = ui->SpeedWind->text().toDouble();
    QString WindSide;

    qDebug() <<"BettaAngle " << BettaAngle;
    qDebug() <<"TimeSite " << TimeSite;
    qDebug() <<"WindSpeed " << WindSpeed;

    const double SpeedShip_Mc = this->SpeedShip.toDouble() / 1.943;
    qDebug() << "Speed ship in m/c" << SpeedShip_Mc;
    double Vi = sqrt(pow(WindSpeed, 2) + pow(SpeedShip_Mc, 2) - 2 * SpeedShip_Mc * WindSpeed * cos(BettaAngle));
    qDebug() <<"Real speed wind m/c: " <<Vi;

    //Check Bofort coefficient
    if(Vi<8.0)
        BeaufortScale = 0;
    if(Vi>=8.0 && Vi<=10.7)
        BeaufortScale = 5;
    if(Vi>=10.8 && Vi<=13.8)
        BeaufortScale = 6;
    if(Vi>=13.9 && Vi<=17.1)
        BeaufortScale = 7;
    if(Vi>=17.2 && Vi<=20.7)
        BeaufortScale = 8;

    if(Vi>=20.8)
        BeaufortScale = 8;

    qDebug() <<"Boford coefficient: " <<BeaufortScale;
    qDebug() << "-----------------\n";
    if(BeaufortScale != 0 && ui->WindHead->isChecked())
    {
        WindCoefficient = 1;
        qDebug() << "Wind coeficient(4%) work";
    }

    if(ui->WindScula->isChecked())
    {
        WindSide = "Scula";
        qDebug() << "Scula wind";
    }
    if(ui->WindHead->isChecked())
    {
        WindSide = "Head";
        qDebug() << "Head wind";
    }
    if(ui->WindBoard->isChecked())
    {
        WindSide = "Board";
        qDebug() << "Board wind";
    }
    if(ui->WindFair->isChecked())
    {
        WindSide = "Fair";
        qDebug() << "Fair wind";
    }
    db.addSite(this->NameShip, BeaufortScale, WindCoefficient, TimeSite, WindSide, this->NameCourse);
    close();
}

void Add_site::on_CencelButton_clicked()
{
    close();
}

void Add_site::on_TimeSite_textChanged(const QString &arg1)
{
    CheckInstertParam();
}

void Add_site::on_BettaAngle_textChanged(const QString &arg1)
{
    CheckInstertParam();
}

void Add_site::on_SpeedWind_textChanged(const QString &arg1)
{
    CheckInstertParam();
}

void Add_site::on_WindFair_clicked()
{
    CheckInstertParam();
}

void Add_site::on_WindScula_clicked()
{
    CheckInstertParam();
}

void Add_site::on_WindHead_clicked()
{
    CheckInstertParam();
}

void Add_site::on_WindBoard_clicked()
{
    CheckInstertParam();
}
