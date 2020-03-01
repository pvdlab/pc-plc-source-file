#include "LoopPlc.h"
#include "Socket.h"
#include "Monitorizza.h"
#include "QDebug"

#define STEP_DEFAUL_TIME 1
#define STEP_PROCESSO_TIME 1


CLoopPlc::CLoopPlc(CCiclo *newCiclo,QString newdbhost,QString newdpuser,QString newdbpassword,int newPorta,QObject *parent) : QThread(parent)
{

    Ciclo=newCiclo;

    if(getPlcListenPort()){
       CServer *PlcSocket = new CServer(Ciclo);
       PlcSocket->start();
    }else{
       MostraErrore("Porta Locale Nulla Impossibile lanciare Server");
    }

    while (getSimula()==-1) {
        msleep(10);
    }

    Ciclo->Macchina_PowOutdig();
    Ciclo->InitScanModulo();
    Simula(Ciclo,STEP_DEFAUL_TIME);

    QMonitorizza *Monitorizza= new QMonitorizza(Ciclo,newdbhost,newdpuser,newdbpassword,newPorta);
    Monitorizza->start();
}



void CLoopPlc::run(){

    for(;;){
         Ciclo->Ciclo_Main();
         Simula.main();
         Ciclo->ScanModulo();
         if(Ciclo->pData[0].ProcessoIsOn)
           msleep(STEP_PROCESSO_TIME);
         else
           msleep(STEP_DEFAUL_TIME);

    }

}



