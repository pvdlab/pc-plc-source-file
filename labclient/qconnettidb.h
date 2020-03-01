#ifndef QCONNETTIDB_H
#define QCONNETTIDB_H

#include <QThread>
#include <QObject>
#include <QSqlDatabase>
#include "qmonitorizza.h"
#include "initdialog.h"

class QConnettiDB : public QThread
{
    Q_OBJECT
public:
    explicit QConnettiDB(CNetDb *newpNet,QObject *parent = 0);
    void run();

signals:
    void VersioneRidotta(bool);
    void mostraEvento(QString msg);
    void mostraErrore(QString msg);
    void Connesso(QSqlDatabase *newDbDati, QSqlDatabase *newDbProcesso,QSqlQuery *queryEvento, QSqlQuery *queryData, QSqlQuery *queryLeek);

private:
    QSqlDatabase *DbDati;
    QSqlDatabase *DbProcesso;
    CNetDb *pNet;
    bool    IsLocale;
    bool PrimaConnessione;
    QMonitorizza *Monitorizza;

};

#endif // QCONNETTIDB_H


