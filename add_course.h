#ifndef ADD_COURSE_H
#define ADD_COURSE_H

#include <QDialog>
#include <Qlist>
#include <list>

namespace Ui {
class Add_course;
}

class Add_course : public QDialog
{
    Q_OBJECT

public:
    explicit Add_course(QWidget *parent = 0, QString NameShip = 0, bool Change = 0, QString NameCourse = 0, QString Speed = 0, QString Distance = 0,  QString Oil = 0, QString Fuel = 0);
    void CheckInstertParam();
    void ViewSite();
    ~Add_course();

private slots:
    void on_CancelButton_clicked();

    void on_AddSite_clicked();

    void on_Distance_textChanged(const QString &arg1);

    void on_SpeedShip_textChanged(const QString &arg1);

    void on_NameCourse_textChanged(const QString &arg1);

    void on_Update_clicked();

    void on_ApplyButton_clicked();

    void on_DeleteSite_clicked();

    void on_Oil_repair_clicked();

    void on_FuelRepair_clicked();

private:
    Ui::Add_course *ui;
    QString NameShip;
};

#endif // ADD_COURSE_H
