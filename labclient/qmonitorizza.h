#ifndef QMONITORIZZA_H
#define QMONITORIZZA_H

#include <QKeyEvent>
#include <QWidget>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include "NetPlc.h"
#include "MainCostanti.h"
#include "qgraficoriquadro.h"



class QMonitorizza : public QWidget
{
    Q_OBJECT

public:
    explicit QMonitorizza(QWidget *parent = 0);
    ~QMonitorizza();
    //void   setDbProcesso(QSqlDbProcesso  newCicloDbProcesso){//CicloDbProcesso=newCicloDbProcesso;
                                                     // Query.finish();}
    void   setEdit(bool newisedit){IsEdit=newisedit;}
    void   Init(QNetPlc *newProcessa);
    void   Edit(int time);
    void   Load(qreal time,QSqlRecord record);
    void   append(int index ,CIOSegnale* pSegnale);
    void   setMaxOut(int index ,qreal max,int);
    void   setTempo(int newTempo);
    void   addTempo(int newTempo);
    void   setQuery(QSqlQuery newqueryData){queryData=newqueryData; lastTempo=0;}
    bool   isActive(){return queryData.isActive();}
    void   gotoZero(int newtempo);
    void   Clear();


public slots:
    void gotoTempo(int x,int newtempo);


private:
    QSqlQuery        Query;
    int              lastTempo;
    bool             IsEdit;
    //QSqlDbProcesso     CicloDbProcesso;
    QGraficoRiquadro *Vuoto;
    QGraficoRiquadro *Riscaldamento;
    QGraficoRiquadro *Gas;
    QGraficoRiquadro *IBias;
    QGraficoRiquadro *VBias;
    QGraficoRiquadro *Catodi;
    QSqlQuery        queryData;

};

#endif // QMONITORIZZA_H
