#include "mainwindow.h"
#include "dialog_add_ship.h"
#include "parametr_ship.h"
#include "ui_mainwindow.h"
#include "db.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
        ViewAllShip();

        //Отмена редактирования таблицы кораблей
        ui->tableAllShips->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //FuelTankChart();
        //OilRealChart();
        MainChart();
        qDebug() << "path database" << path;
        ui->pushButton_2->setStyleSheet("border-image: url(:/Help.jpg) stretch;");
}

void MainWindow::on_AddShip_clicked()
{
    AddShip *d = new AddShip(this);
    d->setModal(true);
    d->show();
}

void MainWindow::on_pushButton_clicked()
{
    QModelIndex currentDiscount  = ui->tableAllShips->currentIndex();
    QString NameShip = (ui->tableAllShips->model()->data(ui->tableAllShips->model()->index(currentDiscount.row(),7),0)).toString();

    DbManager db(path);

    //Удаление строки с выбраным двигателем
    db.deleteShip(NameShip);

    db.CloseConection();
    ViewAllShip();//Обновление таблицы
}

void MainWindow::on_Update_clicked()
{
    ViewAllShip();
    MainChart();
}

void MainWindow::ViewAllShip()
{
    DbManager db(path);

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

        //Заголовки столбцов
        QStringList horizontalHeader;
        horizontalHeader.append("Владелец");
        horizontalHeader.append("Флаг");
        horizontalHeader.append("Высота");
        horizontalHeader.append("Ширина");
        horizontalHeader.append("Длина");
        horizontalHeader.append("Тип корабля");
        horizontalHeader.append("Год выпуска");
        horizontalHeader.append("Имя корабля");

        QSqlQuery query("SELECT Owner, Flag, Height, Width, Length, Type_ship, Year, NameShip FROM Ships ");
        int count = 0;

        //Заголовки строк
        QStringList verticalHeader;


        while (query.next()) {
            QString CalcRow = QString::number(count+1);
            verticalHeader.append(CalcRow);
                 QString Owner = query.value(0).toString();
                 qDebug()<<Owner;
                 QString Flag = query.value(1).toString();
                 QString Height = query.value(2).toString();
                 QString Width = query.value(3).toString();
                 QString Length = query.value(4).toString();
                 QString TypeShip = query.value(5).toString();
                 QString Year = query.value(6).toString();
                 QString NameShip = query.value(7).toString();
                 //Первый ряд
                     item = new QStandardItem(QString(Owner));
                     model->setItem(count, 0, item);

                     item = new QStandardItem(QString(Flag));
                     model->setItem(count, 1, item);

                     item = new QStandardItem(QString(Height));
                     model->setItem(count, 2, item);

                     item = new QStandardItem(QString(Width));
                     model->setItem(count, 3, item);

                     item = new QStandardItem(QString(Length));
                     model->setItem(count, 4, item);

                     item = new QStandardItem(QString(TypeShip));
                     model->setItem(count, 5, item);

                     item = new QStandardItem(QString(Year));
                     model->setItem(count, 6, item);

                     item = new QStandardItem(QString(NameShip));
                     model->setItem(count, 7, item);
                     ++count;
        }

        model->setHorizontalHeaderLabels(horizontalHeader);
        model->setVerticalHeaderLabels(verticalHeader);

        ui->tableAllShips->setModel(model);

        ui->tableAllShips->resizeColumnsToContents();
        ui->tableAllShips->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        db.CloseConection();
}

void MainWindow::MainChart()
{
    DbManager db(path);
    //Clear chart
    ui->FuelTankChart->rescaleAxes();
    ui->FuelTankChart->clearPlottables();

    //Get name ships
    QVector<QString> nameShip;
    QVector<double> ticks;
    QSqlQuery queryNameShip("SELECT NameShip from Ships");
    int count = 0;
    while(queryNameShip.next())
    {
        count++;
        ticks << count;
        qDebug() << "---------------------------";
        nameShip.append(queryNameShip.value(0).toString());
        qDebug() << "Fuel" << nameShip.back();
        qDebug() << "---------------------------";
    }

    // prepare x axis with country labels:

    QVector<QString> labels;
    labels << nameShip;
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
    ui->FuelTankChart->xAxis->setTicker(textTicker);


    // Add data:
    QVector<double> oilData, fuelData;
    for(int i = 0; i < nameShip.size(); i++)
    {
       fuelData.append(FuelTankChart(nameShip[i]));
       oilData.append(OilRealChart(nameShip[i]));
    }

      QCPBarsGroup *group1 = new QCPBarsGroup(ui->FuelTankChart);
      QCPBars *bars = new QCPBars(ui->FuelTankChart->xAxis, ui->FuelTankChart->yAxis);

      bars->setData(ticks, oilData);
      bars->setName("Oil");
      bars->setBrush(QColor(0, 0, 255, 50));
      bars->setPen(QColor(0, 0, 255));
      bars->setWidth(0.15);
      bars->setBarsGroup(group1);
      bars = new QCPBars(ui->FuelTankChart->xAxis, ui->FuelTankChart->yAxis);
      bars->setData(ticks, fuelData);
      bars->setName("Fuel");
      bars->setBrush(QColor(255, 154, 0, 50));
      bars->setPen(QColor(255, 154, 0));
      bars->setWidth(0.15);
      bars->setBarsGroup(group1);
      ui->FuelTankChart->xAxis->setRange(0.1, 5);
      ui->FuelTankChart->yAxis->setRange(0, 100);


      // setup legend:
      ui->FuelTankChart->legend->setVisible(true);
      ui->FuelTankChart->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
      ui->FuelTankChart->legend->setBrush(QColor(255, 255, 255, 100));
      ui->FuelTankChart->legend->setBorderPen(Qt::NoPen);
      QFont legendFont = font();
      legendFont.setPointSize(10);
      ui->FuelTankChart->legend->setFont(legendFont);
      ui->FuelTankChart->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
      ui->FuelTankChart->yAxis->setLabel("Ship fuel and oil consumption");

      ui->FuelTankChart->replot();
    db.CloseConection();

}

double MainWindow::FuelTankChart( QString NameShip )
{

    DbManager db(path);

    //Get date last oil repair of database
    QSqlQuery queryFuelRepair("SELECT Fuel, Date from Course where NameShip like '"+NameShip+"' and Oil != 0 ORDER BY Date DESC Limit 1");
    QString Fuel;
    QString Date;
    int FuelTanker = 0;
    while(queryFuelRepair.next())
    {
        qDebug() << "---------------------------";
        Fuel = queryFuelRepair.value(0).toString();
        Date = queryFuelRepair.value(1).toString();
        FuelTanker = queryFuelRepair.value(0).toInt();
        qDebug() << "Fuel" << Fuel;
        qDebug() << "Date" << Date;
        qDebug() << "---------------------------";
    }

if(FuelTanker!=0){
    //Get date power engine of database
    QSqlQuery queryPower("SELECT Power from Engine where NameShip like '"+NameShip+"'");
    QList<int> PowerEngine;
    while (queryPower.next()) {
             PowerEngine.push_back(queryPower.value(0).toInt());
             qDebug() << "Power engine: " << PowerEngine.back();
    }

    //Get date last oil of database
    QSqlQuery queryFuel("SELECT Real_time_course from Course where NameShip like '"+NameShip+"' and date >= '"+Date+"'");

    int sumConsumption = 0;
    while (queryFuel.next()) {
             double IdealTimeCourse = queryFuel.value(0).toDouble();
             double fuelConsumption = 0;
             for(int i = 0; i < PowerEngine.size(); i++)
             {
                 double g = 0.27/PowerEngine.back();
                 double N = (PowerEngine.back() * g) * PowerEngine.back();
                 fuelConsumption += N * IdealTimeCourse;

                 qDebug() << "Fuel consumption" << fuelConsumption;
             }
             sumConsumption+=fuelConsumption;
    }

    double IdealFuelConsumption = (sumConsumption* 100)/(FuelTanker);

   // qDebug() << "IdealFuelConsumption " << IdealFuelConsumption <<"%";
    return IdealFuelConsumption;
}else{
    return 0;
}
    db.CloseConection();
}

double MainWindow::OilRealChart( QString NameShip)
{
    DbManager db(path);
    //Get date last oil repair of database
    QSqlQuery queryOilRepair("SELECT Oil, Date from Course where NameShip like '"+NameShip+"' and Oil != 0 ORDER BY Date DESC Limit 1");
    QString Oil;
    QString Date;

    while(queryOilRepair.next())
    {
        qDebug() << "---------------------------";
        Oil = queryOilRepair.value(0).toString();
        Date = queryOilRepair.value(1).toString();
        qDebug() << "Oil" << Oil;
        qDebug() << "Date" << Date;
        qDebug() << "---------------------------";
    }

if(Oil!=0)
{
    //Get date last oil of database
    QSqlQuery queryOil("SELECT Oil, Date, Real_time_course from Course where NameShip like '"+NameShip+"' and date >= '"+Date+"'");
    double TimeReal = 0;
    const int Oil_1 = 2000;
    const int Oil_2 = 5000;
    const int Oil_3 = 10000;
    int useOil = 0;

    while(queryOil.next())
    {
        Oil = queryOil.value(0).toString();
        Date = queryOil.value(1).toString();
        TimeReal += queryOil.value(2).toDouble();

        if(Oil == "1")
        {
            useOil = Oil_1;
        }
        if(Oil == "2")
        {
            useOil = Oil_2;
        }
        if(Oil == "3")
        {
            useOil = Oil_3;
        }

        qDebug() << "Oil" << Oil;
        qDebug() << "Date" << Date;
        qDebug() << "---------------------------";
    }

    double RealOilConsumption = (TimeReal * 100)/(useOil);
    return RealOilConsumption;
}else{
     return 0;
}
    db.CloseConection();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tableAllShips_doubleClicked(const QModelIndex &index)
{
        qDebug() << "Double click work";

        QModelIndex currentDiscount  = ui->tableAllShips->currentIndex();
        QString NameShip = (ui->tableAllShips->model()->data(ui->tableAllShips->model()->index(currentDiscount.row(),7),0)).toString();
        qDebug() << "NameShip" << NameShip;

        //Открытие окна параметров рассхода топлива
        Parametr_ship *d = new Parametr_ship(this,NameShip);
        d->setModal(true);
        d->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    // Absolute address of the selected file
    QDesktopServices::openUrl(QUrl::fromLocalFile(qApp->applicationDirPath().append("/Help.pdf")));
}
