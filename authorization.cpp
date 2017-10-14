/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2017 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 04.09.17                                             **
**          Version: 2.0.0                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "authorization.h"
#include "adminform.h"
#include "mainwindow.h"
#include "db.h"
#include "ui_authorization.h"
#include "ui_adminform.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

Authorization::Authorization(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Authorization)
{
  ui->setupUi(this);
}

Authorization::~Authorization()
{
  delete ui;
}

void Authorization::on_pushButton_clicked()
{       
    DbManager db(path);
        QSqlQuery query("SELECT Name, Password FROM Users where Name like '"+ui->NameUser->text()+"' and Password like '"+ui->PasswordUser->text()+"'");

        while (query.next()) {
            QString Name = query.value(0).toString();
            QString Password = query.value(1).toString();
            if(Name == "Adm" &&  Password == "123" )
            {
                qDebug() << "Admin input";
                AdminForm *d = new AdminForm(this);
                d->show();
            }

            if(Name != "Adm" && Name !="")
            {
                qDebug() << "User input";
                MainWindow *d = new MainWindow(this);
                d->setModal(true);
                d->show();
            }
        }
    db.CloseConection();
}

void Authorization::on_pushButton_2_clicked()
{
     exit(1);
}
