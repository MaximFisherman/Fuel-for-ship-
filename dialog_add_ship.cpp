#include "dialog_add_ship.h"
#include "dialog_add_engine.h"
#include "ui_dialog_add_ship.h"
#include "QMessageBox"
#include "db.h"
#include <QFileDialog>

AddShip::AddShip(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddShip)
{
    ui->setupUi(this);
    ui->TypeShip->item(0)->setSelected(true);
}

AddShip::~AddShip()
{
    delete ui;
}


void AddShip::CheckInstertParam()
{
    if(ui->Flag->text() == "" || ui->Owner->text() == "" || ui->Height->text() == "" || ui->Width->text() == "" || ui->Length->text() == "" || ui->Year->text() == "")
    {
        ui->Confirm->setEnabled(false);
    }else{
        ui->Confirm->setEnabled(true);
    }
}

void AddShip::on_AddEngine_clicked()
{
    dialog_add_engine *d = new dialog_add_engine(this,ui->NameShip->text());
    d->setModal(true);
    d->show();
    AddEngine();
}



void AddShip::on_NameShip_textChanged(const QString &arg1)
{
    if(!ui->NameShip->text().isEmpty())
    {
        CheckInstertParam();
        AddEngine();
        EnabledButtonRemove();
        ui->AddEngine->setEnabled(true);
    }else{
        CheckInstertParam();
        AddEngine();
        EnabledButtonRemove();
        ui->AddEngine->setEnabled(false);
    }
}

void AddShip::on_Confirm_clicked()
{
    DbManager db(path);
    const QString& TypeShip = ui->TypeShip->currentItem()->text();
    qDebug()<<"Type ship:" << TypeShip;
    db.addShip( ui->NameShip->text(), ui->Owner->text(), ui->Flag->text(), ui->Height->text(), ui->Width->text(), ui->Length->text(), TypeShip, ui->Year->text() );
    db.CloseConection();
}


void AddShip::AddEngine()
{
    DbManager db(path);

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

        //Заголовки столбцов
        QStringList horizontalHeader;
        horizontalHeader.append("Тип");
        horizontalHeader.append("Крутящий момент");
        horizontalHeader.append("Мощность");
        horizontalHeader.append("Удельный расход топлива");
        horizontalHeader.append("Марка");

        QSqlQuery query("SELECT TypeEngine,Torque, Power, FuelConsumption, MarkaEngine FROM Engine where NameShip like '"+ui->NameShip->text()+"'");
        int count = 0;

        //Заголовки строк
        QStringList verticalHeader;


        while (query.next()) {
            QString CalcRow = QString::number(count+1);
            verticalHeader.append(CalcRow);
                 QString TypeEngine = query.value(0).toString();
                 qDebug()<<TypeEngine;
                 QString Torque = query.value(1).toString();
                 QString Power = query.value(2).toString();
                 QString FuelConsumption = query.value(3).toString();
                 QString MarkaEngine = query.value(4).toString();
                 //Первый ряд
                     item = new QStandardItem(QString(TypeEngine));
                     model->setItem(count, 0, item);

                     item = new QStandardItem(QString(Torque));
                     model->setItem(count, 1, item);

                     item = new QStandardItem(QString(Power));
                     model->setItem(count, 2, item);

                     item = new QStandardItem(QString(FuelConsumption));
                     model->setItem(count, 3, item);

                     item = new QStandardItem(QString(MarkaEngine));
                     model->setItem(count, 4, item);
                     ++count;
        }

        model->setHorizontalHeaderLabels(horizontalHeader);
        model->setVerticalHeaderLabels(verticalHeader);

        ui->tableViewEngine->setModel(model);

        ui->tableViewEngine->resizeRowsToContents();
        ui->tableViewEngine->resizeColumnsToContents();
        db.CloseConection();
}

//Включение кнопки удаления
void AddShip::EnabledButtonRemove()
{
    ui->RemoveEngine->setEnabled(true);
}


/* Проверки на ввод данных в поля */
void AddShip::on_Owner_textChanged(const QString &arg1)
{
    CheckInstertParam();
}

void AddShip::on_Flag_textChanged(const QString &arg1)
{
    CheckInstertParam();
}

void AddShip::on_Year_textChanged(const QString &arg1)
{
     CheckInstertParam();
}

void AddShip::on_Height_textChanged(const QString &arg1)
{
     CheckInstertParam();
}

void AddShip::on_Width_textChanged(const QString &arg1)
{
     CheckInstertParam();
}

void AddShip::on_Length_textChanged(const QString &arg1)
{
     CheckInstertParam();
}

void AddShip::on_pushButton_clicked()
{
    AddEngine();
}

void AddShip::on_RemoveEngine_clicked()
{
    QModelIndex currentDiscount  = ui->tableViewEngine->currentIndex();
    QString MarkaEngine = (ui->tableViewEngine->model()->data(ui->tableViewEngine->model()->index(currentDiscount.row(),4),0)).toString();

    qDebug() << ui->tableViewEngine->model()->data(ui->tableViewEngine->model()->index(currentDiscount.row(),4),0);
    qDebug() << MarkaEngine;
    DbManager db(path);

    //Удаление строки с выбраным двигателем
    db.deleteEngine(MarkaEngine);

    db.CloseConection();
    AddEngine();
}

void AddShip::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(0,QDir::homePath());
    QFile file(path);
    qDebug() << "Path file selected file" << path;

       QString fileName = QFileInfo(path).fileName();
       QString newDir = qApp->applicationDirPath().append("/UserFile/");
       file.copy(newDir + fileName);

    if(path != "")
    {
        QMessageBox msgBox;
        msgBox.setText("Ваш файл сохранен в дериктории программы в файле User file, там вы можете его посмотреть");
        msgBox.exec();
    }


}
