#include <QtCore>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QThread>
#include "qplcprocesso.h"
#include "ccsocket.h"

#define IPSCANTIME 1000

/*#ifdef PER_PC
   QString  EventoPath="/home/ricvb/wandplc";
#else
   QString  EventoPath="/home/ricvb/wandplc";
#endif*/

QString lastevento;
QString lastessore;
QString lastora;
QString lastdes;



// cd /home/ricvb/Dropbox/run/consoleplc ; ./consoleplc
// cd /home/ricvb/Dropbox/run/labsever ; ./labsever

QPlcProcesso::QPlcProcesso(CProcessa *newProcesso,QObject *parent) : QObject(parent)
{
    Processa=newProcesso;


#ifdef TEST_MIOSOKET
    CMioSocket *PlcSocket = new CMioSocket(Processa,true);
    PlcSocket->start();
#else
    Server = new QPlcServer(Processa);
    Server->connetti();
#endif
    Processa->Plc.OpenPort();
    Simula(Processa);

    QTimer *iptimer = new QTimer(this);
    QObject::connect(iptimer, SIGNAL(timeout()),
                     this, SLOT(Scan()));
    iptimer->start(5);


}

void QPlcProcesso::Scan()
{
     Processa->Main();
     if(getSimula())
        Simula.Main();
}


