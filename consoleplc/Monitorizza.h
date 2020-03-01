#ifndef QMONITORIZZA_H
#define QMONITORIZZA_H

#include <QThread>
#include <QObject>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QThreadPool>
#include "Update.h"
#include "StdAfx.h"
#include "Ciclo.h"

class QMonitorizza : public QThread
{
    Q_OBJECT

public:
    explicit QMonitorizza(CCiclo *newProcessa,QString newdbhost,QString newdpuser,QString newdbpassword, int newPorta,QObject *parent = 0);
    void run();
    bool Errore;

public slots:
    void onUpdate(bool);

signals:
    void isErrore(bool);

private:
    void Simula();
    bool Aggiungi();
    void Monitorizza();
    void RecordInit();
    void RecordAggiungi();
    void Update();
    void addSegnale(CIOSegnale *pNewSegnale);


private:
    QSqlDatabase *DataBase;
    QSqlQuery  *queryData;
    CCiclo     *Processa;
    QString    Dbuser;
    QString    Dbpassword;
    QString    Dbhost;
    int        DbPorta;
    int        CicloRecordCount;
    QString    strUpdate;
    QList<CIOSegnale*>   pSegnali;
    int        IdCiclo;
    bool       IsUpdate;
    int        TempoCount;
    int        TempoMax;
    int        VediPer;
    int        BiasGetI;
    CTimer     m_Timer;
    QUpdate    *pUpdate;
    char       Buffer[D_BUFFER];
    int        recordSize;
    int        *Sorgente;
};




#endif // QMONITORIZZA_H
