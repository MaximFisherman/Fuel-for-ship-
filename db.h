#ifndef DB_H
#define DB_H

#include <QtSql>
#include "global_variables.h"
#include "QStandardItemModel"
#include "QStandardItem"

class DbManager
{
public:
    DbManager(const QString& path);
    bool CloseConection();
    bool deleteEngine(const QString& MarkaEngine);
    bool deleteShip(const QString& NameShip);
    bool deleteCourse(const QString& NameShip, const QString& NameCourse);
    bool UpdateCourse(
            const QString& NameShip,
            const QString& NameCourse,
            const double& SpeedShip,
            const double& SredSpeedShip,
            const double& IdealTimeCourse,
            const double& RealTimeCourse,
            const int& Oil,
            const QString& Fuel,
            const QString& Distance,
            const QString& Speed
            );
    bool AddCourse(
            const QString& NameShip,
            const QString& NameCourse,
            const double& SpeedShip,
            const double& SredSpeedShip,
            const double& IdealTimeCourse,
            const double& RealTimeCourse,
            const int& Oil,
            const QString& Fuel,
            const QString& Distance,
            const QString& Speed
            );
    bool addEngine(
            const QString& TypeEngine,
            const QString& Torque,
            const QString& Power,
            const QString& FuelConsumption,
            const QString& NameShip,
            const QString& MarkaEngine
            );

    bool addShip(
            const QString& NameShip,
            const QString& Owner,
            const QString& Flag,
            const QString& Height,
            const QString& Width,
            const QString& Length,
            const QString& TypeShip,
            const QString& Year
            );

    bool addSite(
            const QString& NameShip,
            const int& Beaufort_Scale,
            const int& WindCoefficient,
            const int& TimeSite,
            const QString WindSide,
            const QString NameCourse
            );
private:
    QSqlDatabase m_db;
};
#endif // DB_H
