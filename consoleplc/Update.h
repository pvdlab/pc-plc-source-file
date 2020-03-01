#ifndef QUPDATE_H
#define QUPDATE_H

#include <QObject>
#include <QThread>
#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QRunnable>

class QUpdate : public QObject, public QRunnable
{

    Q_OBJECT
public:
    explicit QUpdate(QSqlDatabase *newDataBase,QString strSql);
    void run();

signals:
    void isUpdate(bool);

private:
    QString      strUpdate;
    QSqlDatabase *DataBase ;

};




#endif // QUPDATE_H
