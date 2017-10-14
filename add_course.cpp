#include "add_course.h"
#include "ui_add_course.h"
#include "add_site.h"
#include "db.h"
#include "ui_add_site.h"

bool flagSite = false;
bool flagChange;

Add_course::Add_course(QWidget *parent, QString NameShip, bool Change, QString NameCourse, QString Speed, QString Distance, QString Oil, QString Fuel) :
    QDialog(parent),
    ui(new Ui::Add_course)
{
    if(Change == true)
    {
        ui->setupUi(this);
        flagChange = true;
        if(Oil == "Залито масло малой продолжительности")
        {
            ui->Oil_repair->setChecked(true);
            ui->Oil_1->setChecked(true);
            ui->Oil_1->setEnabled(true);
        }
        if(Oil == "Залито масло средней продолжительности")
        {
            ui->Oil_repair->setChecked(true);
            ui->Oil_2->setChecked(true);
            ui->Oil_2->setEnabled(true);
        }
        if(Oil == "Залито масло длительной продолжительности")
        {
            ui->Oil_repair->setChecked(true);
            ui->Oil_3->setChecked(true);
            ui->Oil_3->setEnabled(true);
        }
        if(!Fuel.isEmpty())
        {
            ui->FuelRepair->setChecked(true);
            ui->CountFuel->setEnabled(true);
            ui->CountFuel->setText(Fuel);
        }
        this->NameShip = NameShip;
        ui->SpeedShip->setText(Speed);
        ui->Distance->setText(Distance);

        ui->NameCourse->setText(NameCourse);
        ui->NameCourse->setEnabled(false);
        ViewSite();

    }else
    {
        ui->setupUi(this);
        flagChange = false;
        this->NameShip = NameShip;
        ViewSite();
    }
    flagSite = false;
}

Add_course::~Add_course()
{
    delete ui;
}

void Add_course::CheckInstertParam()
{
    if(ui->Distance->text().isEmpty() || ui->SpeedShip->text().isEmpty() || ui->NameCourse->text().isEmpty())
    {
        ui->AddSite->setEnabled(false);
        ui->ApplyButton->setEnabled(false);
    }else{
        ui->AddSite->setEnabled(true);
        ui->ApplyButton->setEnabled(true);
    }
}

void Add_course::on_CancelButton_clicked()
{
    close();
}

void Add_course::on_AddSite_clicked()
{
    ui->NameCourse->setEnabled(false);
    Add_site *d = new Add_site(this, ui->SpeedShip->text(), this->NameShip, ui->NameCourse->text());
    d->setModal(true);
    d->show();
    ViewSite();
    flagSite = true;
}

void Add_course::on_Distance_textChanged(const QString &arg1)
{
    CheckInstertParam();
}

void Add_course::on_SpeedShip_textChanged(const QString &arg1)
{
    CheckInstertParam();
}

void Add_course::on_NameCourse_textChanged(const QString &arg1)
{
    CheckInstertParam();
    ViewSite();
}

void Add_course::on_Update_clicked()
{
    ViewSite();
}

void Add_course::ViewSite()
{
    DbManager db(path);

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

        //Заголовки столбцов
        QStringList horizontalHeader;
        horizontalHeader.append("Время участка");
        horizontalHeader.append("Шкала Бофорда(сила ветра на участке)");
        horizontalHeader.append("Сторона ветра");
        horizontalHeader.append("Имя маршрута");

        QSqlQuery query("SELECT TimeSite, Beaufort_Scale, Wind_side, Name_course FROM Wind where NameShip like '"+this->NameShip+"' and Name_course like '"+ui->NameCourse->text()+"'");
        int count = 0;

        //Заголовки строк
        QStringList verticalHeader;


        while (query.next()) {
            QString CalcRow = QString::number(count+1);
            verticalHeader.append(CalcRow);
                 QString TimeSite = query.value(0).toString();
                 qDebug() << TimeSite;
                 QString Beaufort_Scale = query.value(1).toString();
                 QString Wind_side = query.value(2).toString();
                 QString Name_course = query.value(3).toString();

                 //Первый ряд
                     item = new QStandardItem(QString(TimeSite));
                     model->setItem(count, 0, item);

                     item = new QStandardItem(QString(Beaufort_Scale));
                     model->setItem(count, 1, item);

                     item = new QStandardItem(QString(Wind_side));
                     model->setItem(count, 2, item);

                     item = new QStandardItem(QString(Name_course));
                     model->setItem(count, 3, item);

                     ++count;
        }

        model->setHorizontalHeaderLabels(horizontalHeader);
        model->setVerticalHeaderLabels(verticalHeader);

        ui->TableViewSites->setModel(model);
        ui->TableViewSites->resizeColumnsToContents();
        ui->TableViewSites->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        db.CloseConection();
}





void Add_course::on_ApplyButton_clicked()
{
    qDebug() << "------------------------";
    qDebug() << "Apply start";

    DbManager db(path);
    double LengthShip = 0;
    double SpeedShip = ui->SpeedShip->text().toDouble() * 0.51; //Nodes in m/c
    double RealSpeedSite = 0; //Speed on site
    double Distance = ui->Distance->text().toDouble()*1.852;//Convert to km
    double TimeCourse = Distance/(SpeedShip * 1.852);

    qDebug() << "Common time:" << TimeCourse;

    //Get length ship
    QSqlQuery queryLengthShip("SELECT Length FROM Ships where NameShip like '"+this->NameShip+"'");

    while (queryLengthShip.next()) {
             qDebug() << "Get length ship";
             LengthShip = queryLengthShip.value(0).toDouble();
    }


    QSqlQuery query("SELECT TimeSite, Beaufort_Scale, Wind_side, Name_course, Wind_coeff FROM Wind where NameShip like '"+this->NameShip+"' and Name_course like '"+ui->NameCourse->text()+"'");
    int m = 0, n = 0;
    QList<double> DeltaSpeedShip;
    while (query.next()) {
             qDebug() << "Get date of table Wind";

             double TimeSite = query.value(0).toDouble();
             int Beaufort_Scale = query.value(1).toInt();
             QString Wind_side = query.value(2).toString();
             QString Name_course = query.value(3).toString();
             bool Wind_coefficient = query.value(4).toBool();

             //Buford coefficient 5
             if(Wind_side == "Head" && Beaufort_Scale == 5)
             {
                 qDebug() << "Head wind";
                 m = 800;
                 n = 2;
             }
             if(Wind_side == "Scula" && Beaufort_Scale == 5)
             {
                 qDebug() << "Scula wind";
                 m = 700;
                 n = 2;
             }
             if(Wind_side == "Board" && Beaufort_Scale == 5)
             {
                 qDebug() << "Board wind";
                 m = 350;
                 n = 1;
             }
             if(Wind_side == "Fair" && Beaufort_Scale == 5)
             {
                 qDebug() << "Fair wind";
                 m = 100;
                 n = 0;
             }

             //Buford coefficient 6
             if(Wind_side == "Head" && Beaufort_Scale == 6)
             {
                 qDebug() << "Head wind";
                 m = 1300;
                 n = 6;
             }
             if(Wind_side == "Scula" && Beaufort_Scale == 6)
             {
                 qDebug() << "Scula wind";
                 m = 1000;
                 n = 5;
             }
             if(Wind_side == "Board" && Beaufort_Scale == 6)
             {
                 qDebug() << "Board wind";
                 m = 500;
                 n = 1;
             }
             if(Wind_side == "Fair" && Beaufort_Scale == 6)
             {
                 qDebug() << "Fair wind";
                 m = 200;
                 n = 1;
             }

             // Buford coefficient 7
             if(Wind_side == "Head" && Beaufort_Scale == 7)
             {
                 qDebug() << "Head wind";
                 m = 2100;
                 n = 11;
             }
             if(Wind_side == "Scula" && Beaufort_Scale == 7)
             {
                 qDebug() << "Scula wind";
                 m = 1400;
                 n = 8;
             }
             if(Wind_side == "Board" && Beaufort_Scale == 7)
             {
                 qDebug() << "Board wind";
                 m = 700;
                 n = 5;
             }
             if(Wind_side == "Fair" && Beaufort_Scale == 7)
             {
                 qDebug() << "Fair wind";
                 m = 400;
                 n = 2;
             }

             // Buford coefficient 8
             if(Wind_side == "Head" && Beaufort_Scale == 8)
             {
                 qDebug() << "Head wind";
                 m = 3600;
                 n = 18;
             }
             if(Wind_side == "Scula" && Beaufort_Scale == 8)
             {
                 qDebug() << "Scula wind";
                 m = 2300;
                 n = 12;
             }
             if(Wind_side == "Board" && Beaufort_Scale == 8)
             {
                 qDebug() << "Board wind";
                 m = 1000;
                 n = 7;
             }
             if(Wind_side == "Fair" && Beaufort_Scale == 8)
             {
                 qDebug() << "Fair wind";
                 m = 700;
                 n = 3;
             }


             RealSpeedSite = (((m/LengthShip) + n)*SpeedShip)/100;

             //if head wind then 4% speed down
             if(Wind_coefficient == true)
             {
                  qDebug() << "Calculate wind coefficient";
                  int temp = (RealSpeedSite * 4)/100;
                  RealSpeedSite -=temp;
             }
             DeltaSpeedShip.push_back(RealSpeedSite);
             qDebug() << "Delta speed in m/c: " << RealSpeedSite;
             qDebug() << "Delta speed in nodes: " << RealSpeedSite * 1.94384;
    }

    qDebug() << " ";

    double SredSpeedShip = 0;
    int count = 0;
    while(!DeltaSpeedShip.isEmpty())
    {
        if(DeltaSpeedShip.back() == 0)
        {
            SredSpeedShip += SpeedShip;
        }else
        {
            SredSpeedShip += (double)DeltaSpeedShip.back();
        }

        DeltaSpeedShip.pop_back();
        count++;
    }
    SredSpeedShip = SredSpeedShip/count;

    if(flagSite == false)
    {
        SredSpeedShip = SpeedShip;
    }
    qDebug() << "Flag = " << flagSite << "  SredSpeedShip " << SredSpeedShip;
    qDebug() << "Sred speed ship in nodes" << SredSpeedShip * 1.94384;

    double IdealTimeCourse = Distance/((SpeedShip/1000)*3600);
    double RealTimeCourse = Distance/((SredSpeedShip/1000)*3600);
    qDebug() << "Ideal time course " << IdealTimeCourse;
    qDebug() << "Real time course " << RealTimeCourse;

    int Oil = 0;
    QString Fuel = 0;
    //Check oil repair
    if(ui->Oil_repair->isChecked())
    {
        if(ui->Oil_1->isChecked()) Oil = 1;
        if(ui->Oil_2->isChecked()) Oil = 2;
        if(ui->Oil_3->isChecked()) Oil = 3;
    }
    if(!ui->CountFuel->text().isEmpty())
    {
        qDebug()<< "Count litre fuel" << ui->CountFuel->text();
        Fuel = ui->CountFuel->text();
    }

    if(flagChange == false)
    {
        db.AddCourse(this->NameShip, ui->NameCourse->text(), SpeedShip, SredSpeedShip, IdealTimeCourse, RealTimeCourse, Oil, Fuel, ui->Distance->text(), ui->SpeedShip->text());
    }else
    {
        qDebug() << "Speed ship = " << ui->SpeedShip->text();
        db.UpdateCourse(this->NameShip, ui->NameCourse->text(), SpeedShip, SredSpeedShip, IdealTimeCourse, RealTimeCourse, Oil, Fuel, ui->Distance->text(),  ui->SpeedShip->text());
    }
    db.CloseConection();
    close();
}

void Add_course::on_DeleteSite_clicked()
{
    QModelIndex currentDiscount  = ui->TableViewSites->currentIndex();
    QString TimeSite = (ui->TableViewSites->model()->data(ui->TableViewSites->model()->index(currentDiscount.row(),0),0)).toString();
    QString BeaufordScale = (ui->TableViewSites->model()->data(ui->TableViewSites->model()->index(currentDiscount.row(),1),0)).toString();
    QString SideWind = (ui->TableViewSites->model()->data(ui->TableViewSites->model()->index(currentDiscount.row(),2),0)).toString();
    QString NameCourse = (ui->TableViewSites->model()->data(ui->TableViewSites->model()->index(currentDiscount.row(),3),0)).toString();

    qDebug() << "Time site "<<TimeSite;
    qDebug() << "BeauffordScale "<<BeaufordScale;
    qDebug() << "SideWind "<<SideWind;
    qDebug() << "NameCourse "<<NameCourse;

    DbManager db(path);
    QSqlQuery queryDeleteSite;
    queryDeleteSite.prepare( "DELETE FROM Wind WHERE Name_course like '"+NameCourse+"' and TimeSite = "+TimeSite+" and Beaufort_Scale = "+BeaufordScale+" and Wind_side like '%"+SideWind+"%';" );
      if( !queryDeleteSite.exec() ){
            qDebug() << queryDeleteSite.lastError();
        }else{
            qDebug( "Site record deleted!" );
        }
     ViewSite();
    db.CloseConection();
}

void Add_course::on_Oil_repair_clicked()
{
    if(ui->Oil_repair->isChecked())
    {
        ui->Oil_1->setEnabled(true);
        ui->Oil_2->setEnabled(true);
        ui->Oil_3->setEnabled(true);
    }
    else
    {
        ui->Oil_1->setEnabled(false);
        ui->Oil_2->setEnabled(false);
        ui->Oil_3->setEnabled(false);
    }
}

void Add_course::on_FuelRepair_clicked()
{
    if(ui->FuelRepair->isChecked())
    {
       ui->CountFuel->setEnabled(true);
    }
    else
    {
       ui->CountFuel->setEnabled(false);
    }
}
