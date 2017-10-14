#ifndef PARAMETR_SHIP_H
#define PARAMETR_SHIP_H

#include <QDialog>

namespace Ui {
class Parametr_ship;
}

class Parametr_ship : public QDialog
{
    Q_OBJECT

public:
    explicit Parametr_ship(QWidget *parent = 0, QString NameShip = 0);
    void ViewShipCourses();
    double OilRealChart();
    double OilIdealChart();
    void FuelShipChart();
    double FuelTankChart();
    void MainChartResourse();
    ~Parametr_ship();

private slots:
    void on_AddCourse_clicked();

    void on_pushButton_clicked();

    void on_RemoveRecordCourse_clicked();

    void on_TableViewCourses_doubleClicked(const QModelIndex &index);

    void on_TableViewCourses_clicked(const QModelIndex &index);

private:
    Ui::Parametr_ship *ui;
    QString NameShip;
};

#endif // PARAMETR_SHIP_H
