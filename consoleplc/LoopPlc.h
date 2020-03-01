#ifndef CLOOPPLC_H
#define CLOOPPLC_H

#include <QThread>
#include "StdAfx.h"
#include "Simula.h"
#include "Ciclo.h"

class CLoopPlc : public QThread
{
public:
    explicit CLoopPlc(CCiclo *newCiclo,QString newdbhost,QString newdpuser,QString newdbpassword,int newPorta,QObject *parent = 0);
    void run();


private:
    CCiclo       *Ciclo;
    CSimula      Simula;

};


#endif // CLoopPlc_H
