#ifndef ADD_SITE_H
#define ADD_SITE_H

#include <QDialog>

namespace Ui {
class Add_site;
}

class Add_site : public QDialog
{
    Q_OBJECT

public:
    explicit Add_site(QWidget *parent = 0, QString SpeedShip = 0, QString NameShip = 0, QString NameCourse = 0);
    void CheckInstertParam();
    ~Add_site();

private slots:

    void on_AcceptedButton_clicked();

    void on_CencelButton_clicked();

    void on_TimeSite_textChanged(const QString &arg1);

    void on_BettaAngle_textChanged(const QString &arg1);

    void on_SpeedWind_textChanged(const QString &arg1);

    void on_WindFair_clicked();

    void on_WindScula_clicked();

    void on_WindHead_clicked();

    void on_WindBoard_clicked();

private:
    Ui::Add_site *ui;
    QString NameShip;
    QString SpeedShip;
    QString NameCourse;
};

#endif // ADD_SITE_H
