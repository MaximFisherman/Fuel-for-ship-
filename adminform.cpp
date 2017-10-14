#include "adminform.h"
#include "ui_adminform.h"
#include <QMessageBox>

AdminForm::AdminForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminForm)
{
    ui->setupUi(this);
    ViewUser();
}

AdminForm::~AdminForm()
{
    delete ui;
}

void AdminForm::ViewUser()
{
    DbManager db(path);

    QStandardItemModel *model = new QStandardItemModel;
    QStandardItem *item;

        //Заголовки столбцов
        QStringList horizontalHeader;
        horizontalHeader.append("Имя");
        horizontalHeader.append("Пароль");

        QSqlQuery query("SELECT Name, Password FROM Users ");
        int count = 0;

        //Заголовки строк
        QStringList verticalHeader;


        while (query.next()) {
            QString CalcRow = QString::number(count+1);
            verticalHeader.append(CalcRow);
                 QString Owner = query.value(0).toString();
                 qDebug()<<Owner;
                 QString Flag = query.value(1).toString();
                 //Первый ряд
                     item = new QStandardItem(QString(Owner));
                     model->setItem(count, 0, item);

                     item = new QStandardItem(QString(Flag));
                     model->setItem(count, 1, item);
                     ++count;
        }

        model->setHorizontalHeaderLabels(horizontalHeader);
        model->setVerticalHeaderLabels(verticalHeader);

        ui->tableUser->setModel(model);

        ui->tableUser->resizeColumnsToContents();
        ui->tableUser->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        db.CloseConection();
}


void AdminForm::on_AddUser_clicked()
{
    DbManager db(path);
    QSqlQuery query;
    query.prepare("INSERT INTO Users (Name, Password) VALUES (:Name, :Password);");
    query.bindValue(":Name", ui->UserName->text());
    query.bindValue(":Password", ui->UserPassword->text());
    if(query.exec())
    {
        qDebug() << "User was adding";
    }
    else
    {
         qDebug() << "User not added, error:  "
                  << query.lastError();
    }
    db.CloseConection();
        ViewUser();
}

void AdminForm::on_DeleteUser_clicked()
{
    QModelIndex currentDiscount  = ui->tableUser->currentIndex();
    QString NameUser = (ui->tableUser->model()->data(ui->tableUser->model()->index(currentDiscount.row(),0),0)).toString();

    qDebug() << ui->tableUser->model()->data(ui->tableUser->model()->index(currentDiscount.row(),0),0);
    qDebug() << NameUser;

    DbManager db(path);
    QSqlQuery queryDeleteWind;
    if(NameUser != "Adm")
    {

    queryDeleteWind.prepare( "DELETE FROM Users where Name like '"+NameUser+"';" );
       if( !queryDeleteWind.exec() ){
             qDebug() << queryDeleteWind.lastError();
         }else{
             qDebug( "User was deleting!" );
         }
    }else
    {
        qDebug() << "Admin not deleted";
        QMessageBox msgBox;
        msgBox.setText("Админа удалять нельзя");
        msgBox.exec();
    }
    db.CloseConection();
    ViewUser();
}
