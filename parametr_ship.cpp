#include "parametr_ship.h"
#include "add_course.h"
#include "ui_parametr_ship.h"
#include "ui_add_course.h"
#include "db.h"
#include <QSharedPointer>
#include <vector>

static int countRequest = 0;


Parametr_ship::Parametr_ship(QWidget *parent, QString NameShip) :
    QDialog(parent),
    ui(new Ui::Parametr_ship)
{
    qDebug() <<"Form two get param name ship: "<<NameShip;
    ui->setupUi(this);
    this->setWindowTitle(NameShip);
    this->NameShip = NameShip;

    ViewShipCourses();

    MainChartResourse();
    FuelShipChart();
}

Parametr_ship::~Parametr_ship()
{
    delete ui;
}

void Parametr_ship::on_AddCourse_clicked()
{
    Add_course *d = new Add_course(this, this->NameShip, 0, "", "", "");
    d->setModal(true);
    d->show();
}

void Parametr_ship::on_pushButton_clicked()
{
    ViewShipCourses();
    MainChartResourse();
    FuelShipChart();
}

void Parametr_ship::ViewShipCourses()
{
    DbManager db(path);

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

        //Заголовки столбцов
        QStringList horizontalHeader;
        horizontalHeader.append("Название курса");
        horizontalHeader.append("Средняя скорость на маршруте");
        horizontalHeader.append("Дистанция");
        horizontalHeader.append("Топливо");
        horizontalHeader.append("Масло");

        QSqlQuery query("SELECT Name_course, Speed, Size_course, Oil, Fuel FROM Course where NameShip like '"+this->NameShip+"' ORDER BY Date ");
        int count = 0;

        //Заголовки строк
        QStringList verticalHeader;


        while (query.next()) {
            QString CalcRow = QString::number(count+1);
            verticalHeader.append(CalcRow);
                 QString Name_course = query.value(0).toString();

                 QString Speed = query.value(1).toString();
                 qDebug() << "Speed ship " << Speed;
                 QString Distance = query.value(2).toString();
                 QString Oil = query.value(3).toString();
                 QString Fuel = query.value(4).toString();

                 //Первый ряд
                     item = new QStandardItem(QString(Name_course));
                     model->setItem(count, 0, item);

                     item = new QStandardItem(Speed + " узлов");
                     model->setItem(count, 1, item);

                     item = new QStandardItem(QString(Distance)+" морские мили");
                     model->setItem(count, 2, item);

                     item = new QStandardItem(QString(Fuel));
                     model->setItem(count, 3, item);

                     QString setOil;
                     if(Oil == "1") setOil = "Залито масло малой продолжительности";
                     if(Oil == "2") setOil = "Залито масло средней продолжительности";
                     if(Oil == "3") setOil = "Залито масло длительной продолжительности";

                     item = new QStandardItem(QString(setOil));
                     model->setItem(count, 4, item);

                     ++count;
        }

        model->setHorizontalHeaderLabels(horizontalHeader);
        model->setVerticalHeaderLabels(verticalHeader);

        ui->TableViewCourses->setModel(model);
        ui->TableViewCourses->resizeColumnsToContents();
        ui->TableViewCourses->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        query.clear();
        db.CloseConection();

        MainChartResourse();
}

void Parametr_ship::on_RemoveRecordCourse_clicked()
{
    QModelIndex currentDiscount  = ui->TableViewCourses->currentIndex();
    QString NameCourse = (ui->TableViewCourses->model()->data(ui->TableViewCourses->model()->index(currentDiscount.row(),0),0)).toString();

    qDebug() << ui->TableViewCourses->model()->data(ui->TableViewCourses->model()->index(currentDiscount.row(),0),0);
    qDebug() << NameCourse;

    DbManager db(path);
    //Удаление строки с выбраным двигателем
    db.deleteCourse(this->NameShip, NameCourse);
    QSqlQuery queryDeleteWind;
    queryDeleteWind.prepare( "DELETE FROM Wind WHERE NameShip like '"+this->NameShip+"' and Name_course like '"+NameCourse+"';" );
       if( !queryDeleteWind.exec() ){
             qDebug() << queryDeleteWind.lastError();
         }else{
             qDebug( "Site deleted!" );
         }
    db.CloseConection();
    ViewShipCourses();
    MainChartResourse();
    FuelShipChart();
}

void Parametr_ship::on_TableViewCourses_doubleClicked(const QModelIndex &index)
{
    QModelIndex currentDiscount  = ui->TableViewCourses->currentIndex();
    QString NameCourse = (ui->TableViewCourses->model()->data(ui->TableViewCourses->model()->index(currentDiscount.row(),0),0)).toString();
    QString Speed = (ui->TableViewCourses->model()->data(ui->TableViewCourses->model()->index(currentDiscount.row(),1),0)).toString();
    QString Distance = (ui->TableViewCourses->model()->data(ui->TableViewCourses->model()->index(currentDiscount.row(),2),0)).toString();
    QString Fuel = (ui->TableViewCourses->model()->data(ui->TableViewCourses->model()->index(currentDiscount.row(),3),0)).toString();
    QString Oil = (ui->TableViewCourses->model()->data(ui->TableViewCourses->model()->index(currentDiscount.row(),4),0)).toString();
    qDebug() << " ";
    qDebug() << " ";
    qDebug() << "Change course start";
    Speed.chop(6);
    Distance.chop(12);
    Add_course *d = new Add_course(this, this->NameShip, 1, NameCourse, Speed, Distance , Oil, Fuel);
    d->setModal(true);
    d->show();
}

void Parametr_ship::MainChartResourse()
{
    //Clear chart
    ui->MainChart->rescaleAxes();
    ui->MainChart->clearPlottables();

    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 200);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->MainChart->setBackground(QBrush(gradient));

    // create empty bar chart objects:
    QCPBars *Bars = new QCPBars(ui->MainChart->xAxis,ui->MainChart->yAxis);
    Bars->setAntialiased(false); // gives more crisp, pixel aligned bar borders

    Bars->setName("Fossil fuels");
    Bars->setPen(QPen(QColor(100, 0, 0).lighter(100)));
    Bars->setBrush(QColor(100, 0, 0));
    // prepare x axis with country labels:
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 1 << 2 << 3;
    labels <<"Реальное масло" << "Иделальное масло" << "Топливо";
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);
   ui->MainChart->xAxis->setTicker(textTicker);
   ui->MainChart->xAxis->setTickLabelRotation(60);
   ui->MainChart->xAxis->setSubTicks(false);
   ui->MainChart->xAxis->setTickLength(0, 4);
   ui->MainChart->xAxis->setRange(0, 8);
   ui->MainChart->xAxis->setBasePen(QPen(Qt::white));
   ui->MainChart->xAxis->setTickPen(QPen(Qt::white));
   ui->MainChart->xAxis->grid()->setVisible(true);
   ui->MainChart->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
   ui->MainChart->xAxis->setTickLabelColor(Qt::white);
   ui->MainChart->xAxis->setLabelColor(Qt::white);

    // prepare y axis:
   ui->MainChart->yAxis->setRange(0, 100);
   ui->MainChart->yAxis->setPadding(5); // a bit more space to the left border
   ui->MainChart->yAxis->setLabel("Затраты топлива и масла на рейсе");
   ui->MainChart->yAxis->setBasePen(QPen(Qt::white));
   ui->MainChart->yAxis->setTickPen(QPen(Qt::white));
   ui->MainChart->yAxis->setSubTickPen(QPen(Qt::white));
   ui->MainChart->yAxis->grid()->setSubGridVisible(true);
   ui->MainChart->yAxis->setTickLabelColor(Qt::white);
   ui->MainChart->yAxis->setLabelColor(Qt::white);
   ui->MainChart->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
   ui->MainChart->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Add data:
    QVector<double> Date;
    Date  << OilRealChart() << OilIdealChart() << FuelTankChart();

    Bars->setData(ticks, Date);

    // setup legend:
   //ui->MainChart->legend->setVisible(true);
   ui->MainChart->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
   ui->MainChart->legend->setBrush(QColor(255, 255, 255, 100));
   ui->MainChart->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
   ui->MainChart->legend->setFont(legendFont);
   ui->MainChart->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

   ui->MainChart->replot();
}


double Parametr_ship::OilRealChart()
{
    DbManager db(path);
    //Get date last oil repair of database
    QSqlQuery queryOilRepair("SELECT Oil, Date from Course where NameShip like '"+this->NameShip+"' and Oil != 0 ORDER BY Date DESC Limit 1");
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
    QSqlQuery queryOil("SELECT Oil, Date, Real_time_course from Course where NameShip like '"+this->NameShip+"' and date >= '"+Date+"'");
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
    qDebug() << "RealOilConsumption " << RealOilConsumption <<"%";
    return RealOilConsumption;
}else{
    return 0;
}
    db.CloseConection();
}

double Parametr_ship::OilIdealChart()
{
    DbManager db(path);
    //Get date last oil repair of database
    QSqlQuery queryOilRepair("SELECT Oil, Date from Course where NameShip like '"+this->NameShip+"' and Oil != 0 ORDER BY Date DESC Limit 1");
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
    QSqlQuery queryOil("SELECT Oil, Date, Ideal_time_course from Course where NameShip like '"+this->NameShip+"' and date >= '"+Date+"'");
    double TimeIdeal = 0;
    const int Oil_1 = 2000;
    const int Oil_2 = 5000;
    const int Oil_3 = 10000;
    int useOil = 0;

    while(queryOil.next())
    {
        Oil = queryOil.value(0).toString();
        Date = queryOil.value(1).toString();
        TimeIdeal += queryOil.value(2).toDouble();

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

    double IdealOilConsumption = (TimeIdeal * 100)/(useOil);
    qDebug() << "IdealOilConsumption " << IdealOilConsumption <<"%";
    return IdealOilConsumption;
}else{
    return 0;
}
    db.CloseConection();
}

void Parametr_ship::FuelShipChart()
{

        DbManager db(path);

        //Get date power engine of database
        QSqlQuery queryPower("SELECT Power from Engine where NameShip like '"+this->NameShip+"'");
        QList<int> PowerEngine;
        while (queryPower.next()) {
                 PowerEngine.push_back(queryPower.value(0).toInt());
                 qDebug() << "Power engine: " << PowerEngine.back();
        }


        ui->FuelCourses->setNoAntialiasingOnDrag(true); // more performance/responsiveness during dragging
        ui->FuelCourses->addGraph();
        QPen pen;
        pen.setColor(QColor(255,170,100));
        pen.setWidth(2);
        pen.setStyle(Qt::DotLine);

            QVector<double> x, y; //Массивы координат точек
            x.append(0); y.append(0);
            //Зададим наши точки

            QSqlQuery queryCourseReal("SELECT Real_time_course from Course where NameShip like '"+this->NameShip+"'");
            int countCoursesReal = 0;
            QVector<QPointF> pointsReal;
            pointsReal.append(QPointF(0, 0));
            int sumConsumptionReal = 0;
            while (queryCourseReal.next()) {
                countCoursesReal++ ;
                     double RealTimeCourse = queryCourseReal.value(0).toDouble();
                     double fuelConsumption = 0;
                     for(int i = 0; i < PowerEngine.size(); i++)
                     {
                         double g = 0.27/PowerEngine.back();
                         double N = (PowerEngine.back() * g) * PowerEngine.back();
                         fuelConsumption += N * RealTimeCourse;

                         qDebug() << "Fuel real consumption" << fuelConsumption;
                     }
                     sumConsumptionReal+=fuelConsumption;
                     // Добавить точки
                     x.append(countCoursesReal);
                     y.append(sumConsumptionReal);
            }





            ui->FuelCourses->clearGraphs();//Если нужно, но очищаем все графики
            //Добавляем один график в widget
            ui->FuelCourses->addGraph();
            //Говорим, что отрисовать нужно график по нашим двум массивам x и y
            ui->FuelCourses->graph(0)->setData(x, y);
            ui->FuelCourses->graph(0)->setName("Рельный расход топлива");
            ui->FuelCourses->graph(0)->setPen(QColor(40, 50, 50, 255));//задаем цвет точки
            //формируем вид точек
            ui->FuelCourses->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

            x.clear();
            y.clear();
            x.append(0); y.append(0);


            //Get date ideal time course of database
            QSqlQuery queryCourse("SELECT Ideal_time_course from Course where NameShip like '"+this->NameShip+"'");
            int countCouses = 0;
            int sumConsumption = 0;
            while (queryCourse.next()) {
                countCouses++ ;
                     double IdealTimeCourse = queryCourse.value(0).toDouble();
                     double fuelConsumption = 0;
                     for(int i = 0; i < PowerEngine.size(); i++)
                     {
                         double g = 0.27/PowerEngine.back();
                         double N = (PowerEngine.back() * g) * PowerEngine.back();
                         fuelConsumption += N * IdealTimeCourse;

                         qDebug() << "Fuel consumption" << fuelConsumption;
                     }
                     sumConsumption+=fuelConsumption;
                     // Добавить точки на ранее созданную кривую
                     x.append(countCouses);
                     y.append(sumConsumption);
            }

            //Добавляем один график в widget
            ui->FuelCourses->addGraph();

            //Говорим, что отрисовать нужно график по нашим двум массивам x и y
            ui->FuelCourses->graph(1)->setPen(QPen(Qt::red));
            ui->FuelCourses->graph(1)->setBrush(QBrush(QColor(255, 0, 0, 20)));

            ui->FuelCourses->graph(1)->setData(x, y);
            ui->FuelCourses->graph(1)->setName("Идеальный расход топлива");
                        //формируем вид точек
                        ui->FuelCourses->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));




            //Подписываем оси Ox и Oy
            ui->FuelCourses->xAxis->setLabel("Рейсы");
            ui->FuelCourses->yAxis->setLabel("Количество топлива");


            //И перерисуем график на нашем widget
            ui->FuelCourses->replot();

            //Установим область, которая будет показываться на графике
            ui->FuelCourses->xAxis->setRange(0, 10);//Для оси Ox
            ui->FuelCourses->yAxis->setRange(0, 50000);//Для оси Oy

        // make range draggable and zoomable:
        ui->FuelCourses->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        ui->FuelCourses->yAxis->grid()->setSubGridVisible(true);
        ui->FuelCourses->xAxis->grid()->setSubGridVisible(true);
        // make top right axes clones of bottom left axes:
        ui->FuelCourses->axisRect()->setupFullAxesBox();
        // connect signals so top and right axes move in sync with bottom and left axes:
        connect(ui->FuelCourses->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->FuelCourses->xAxis2, SLOT(setRange(QCPRange)));
        connect(ui->FuelCourses->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->FuelCourses->yAxis2, SLOT(setRange(QCPRange)));

        ui->FuelCourses->legend->setVisible(true);
        ui->FuelCourses->legend->setBrush(QBrush(QColor(255,255,255,150)));
        ui->FuelCourses->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // make legend align in top left corner or axis rect
        db.CloseConection();
}


double Parametr_ship::FuelTankChart()
{
    DbManager db(path);

    //Get date last oil repair of database
    QSqlQuery queryFuelRepair("SELECT Fuel, Date from Course where NameShip like '"+this->NameShip+"' and Oil != 0 ORDER BY Date DESC Limit 1");
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
    QSqlQuery queryPower("SELECT Power from Engine where NameShip like '"+this->NameShip+"'");
    QList<int> PowerEngine;
    while (queryPower.next()) {
             PowerEngine.push_back(queryPower.value(0).toInt());
             qDebug() << "Power engine: " << PowerEngine.back();
    }

    //Get date last oil of database
    QSqlQuery queryFuel("SELECT Real_time_course from Course where NameShip like '"+this->NameShip+"' and date >= '"+Date+"'");

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

void Parametr_ship::on_TableViewCourses_clicked(const QModelIndex &index)
{

}
