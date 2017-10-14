#include "db.h"
#include <vector>
#include <QtSql>


DbManager::DbManager(const QString& path)
{
   m_db = QSqlDatabase::addDatabase("QSQLITE");
   m_db.setDatabaseName(path);

   if (!m_db.open())
   {
      qDebug() << "Error: connection with database fail";
   }
   else
   {
      qDebug() << "Database: connection ok";
   }
}

bool DbManager::CloseConection()
{
      m_db.setDatabaseName(path);
      m_db.close();
}

bool DbManager::UpdateCourse(
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
        )
{
    bool success = false;
      // you should check if args are ok first...
      QSqlQuery query;
      qDebug() << "Speed ship" << Speed;
      query.prepare("UPDATE Course SET NameShip = :NameShip, Name_course = :Name_course, Speed_ship = :Speed_ship, SredSpeed_ship = :SredSpeed_ship, Ideal_time_course = :Ideal_time_course, Real_time_course = :Real_time_course, Oil = :Oil, Fuel = :Fuel, Size_course = :Size_course, Speed = :Speed;");
      query.bindValue(":NameShip", NameShip);
      query.bindValue(":Name_course", NameCourse);
      query.bindValue(":Speed_ship", QString::number(SpeedShip));
      query.bindValue(":SredSpeed_ship", QString::number(SredSpeedShip));
      query.bindValue(":Ideal_time_course", IdealTimeCourse);
      query.bindValue(":Real_time_course", RealTimeCourse);
      query.bindValue(":Oil", Oil);
      query.bindValue(":Fuel", Fuel);
      query.bindValue(":Size_course", Distance);
      query.bindValue(":Speed", Speed);

      if(query.exec())
      {
          qDebug() << "Date update done";
          success = true;
      }
      else
      {
           qDebug() << "Course table error:  "
                    << query.lastError();
      }
      return success;
}

bool DbManager::AddCourse(
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
        )
{
    bool success = false;
      // you should check if args are ok first...
      QSqlQuery query;

      query.prepare("INSERT INTO Course (NameShip, Name_course, Speed_ship, SredSpeed_ship, Ideal_time_course, Real_time_course, Oil, Fuel, Size_course, Speed, Date) VALUES (:NameShip, :Name_course, :Speed_ship, :SredSpeed_ship, :Ideal_time_course, :Real_time_course, :Oil, :Fuel, :Size_course, :Speed, datetime('now'));");
      query.bindValue(":NameShip", NameShip);
      query.bindValue(":Name_course", NameCourse);
      query.bindValue(":Speed_ship", QString::number(SpeedShip));
      query.bindValue(":SredSpeed_ship", QString::number(SredSpeedShip));
      query.bindValue(":Ideal_time_course", IdealTimeCourse);
      query.bindValue(":Real_time_course", RealTimeCourse);
      query.bindValue(":Oil", Oil);
      query.bindValue(":Fuel", Fuel);
      query.bindValue(":Size_course", Distance);
      query.bindValue(":Speed", Speed);

      if(query.exec())
      {
          qDebug() << "Date included done";
          success = true;
      }
      else
      {
           qDebug() << "Course table error:  "
                    << query.lastError();
      }
      return success;
}

bool DbManager::addSite(
        const QString& NameShip,
        const int& BeaufortScale,
        const int& WindCoefficient,
        const int& TimeSite,
        const QString WindSide,
        const QString NameCourse
        )
{
    bool success = false;
      // you should check if args are ok first...
      QSqlQuery query;

      query.prepare("INSERT INTO Wind (NameShip, TimeSite, Beaufort_Scale, Wind_coeff, Wind_side, Name_course) VALUES (:NameShip, :TimeSite, :Beaufort_Scale, :Wind_coeff, :Wind_side, :Name_course);");
      query.bindValue(":NameShip", NameShip);
      query.bindValue(":TimeSite", TimeSite);
      query.bindValue(":Beaufort_Scale", BeaufortScale);
      query.bindValue(":Wind_coeff", WindCoefficient);
      query.bindValue(":Wind_side", WindSide);
      query.bindValue(":Name_course", NameCourse);

      if(query.exec())
      {
          success = true;
      }
      else
      {
           qDebug() << "Wind table error:  "
                    << query.lastError();
      }
      return success;
}

bool DbManager::deleteEngine(const QString& MarkaEngine)
{
    QSqlQuery query;

    query.prepare( "DELETE FROM Engine WHERE MarkaEngine like '%"+MarkaEngine+"%';" );
      if( !query.exec() ){
            qDebug() << query.lastError();
            return false;
        }else{
            qDebug( "Deleted!" );
            return true;
        }
}

bool DbManager::deleteCourse(const QString& NameShip, const QString& NameCourse)
{
    QSqlQuery queryDeleteCourse;

    qDebug() <<"Name ship: "<< NameShip;
    qDebug() <<"Name course: "<< NameShip;

    queryDeleteCourse.prepare( "DELETE FROM Course WHERE NameShip like '"+NameShip+"' and Name_course like '"+NameCourse+"';" );
      if( !queryDeleteCourse.exec() ){
            qDebug() << queryDeleteCourse.lastError();
            return false;
        }else{
            qDebug( "Course record deleted!" );
            return true;
        }
}

bool DbManager::deleteShip(const QString& NameShip)
{
    QSqlQuery query;

    qDebug() <<"Name ship: "<< NameShip;
    query.prepare( "DELETE FROM Ships WHERE NameShip like '%"+NameShip+"%';" );
      if( !query.exec() ){
            qDebug() << query.lastError();
            return false;
        }else{
            qDebug( "Deleted!" );
            return true;
        }
}

bool DbManager::addEngine(
        const QString& TypeEngine,
        const QString& Torque,
        const QString& Power,
        const QString& FuelConsumption,
        const QString& NameShip,
        const QString& MarkaEngine
        )
{
    bool success = false;
      // you should check if args are ok first...
      QSqlQuery query;

      query.prepare("INSERT INTO Engine (TypeEngine, Torque, Power, FuelConsumption, NameShip, MarkaEngine) VALUES (:TypeEngine, :Torque, :Power, :FuelConsumption, :Name, :MarkaEngine);");
      query.bindValue(":TypeEngine", TypeEngine);
      query.bindValue(":Torque", Torque);
      query.bindValue(":Power", Power);
      query.bindValue(":FuelConsumption", FuelConsumption);
      query.bindValue(":Name", NameShip);
      query.bindValue(":MarkaEngine", MarkaEngine);

      if(query.exec())
      {
          success = true;
      }
      else
      {
           qDebug() << "Engine table error:  "
                    << query.lastError();
      }
      return success;
}

bool DbManager::addShip(
        const QString& NameShip,
        const QString& Owner,
        const QString& Flag,
        const QString& Height,
        const QString& Width,
        const QString& Length,
        const QString& TypeShip,
        const QString& Year
        )
{
    bool success = false;
      // you should check if args are ok first...
      QSqlQuery query;
        qDebug()<<NameShip;
      query.prepare("INSERT INTO Ships (NameShip, Owner, Flag, Height, Width, Length, Type_Ship, Year) VALUES (:Name, :Owner, :Flag, :Height, :Width, :Length, :Type_Ship, :Year);");
      query.bindValue(":Name", NameShip);
      query.bindValue(":Owner", Owner);
      query.bindValue(":Flag", Flag);
      query.bindValue(":Height", Height.toInt());
      query.bindValue(":Width", Width.toInt());
      query.bindValue(":Length", Length.toInt());
      query.bindValue(":Type_Ship", TypeShip);
      query.bindValue(":Year", Year);



      //query.exec("INSERT INTO test (number) VALUES (23);");
      //query.addBindValue(14);

      //query.exec("INSERT INTO Ships (Owner, Flag, Height, Width, Length, Type_Ship, Year) VALUES ('Santi ANNO12', 'Brazill', 100, 2000, 50, 'Fuel tank','1992')");

      if(query.exec())
      {
          success = true;
      }
      else
      {
           qDebug() << "addShip error:  "
                    << query.lastError();
      }

      return success;
}

