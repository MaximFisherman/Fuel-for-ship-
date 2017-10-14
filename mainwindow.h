#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QModelIndex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void ViewAllShip();
    double FuelTankChart(QString NameShip);
    double OilRealChart(QString NameShip);
    void MainChart();
    ~MainWindow();

private slots:
    void on_AddShip_clicked();

    void on_pushButton_clicked();

    void on_Update_clicked();

    void on_tableAllShips_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QString NameShip;
};

#endif // MAINWINDOW_H
