#include "qlabservernet.h"
#include <QFileSystemWatcher>
#include <QHostInfo>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QNetworkInterface>
#include <QMessageBox>
#include "cthreadclient.h"


QLabserverNet::QLabserverNet(QNetPlc *newProcessa,QString newHost, int newPorta,QObject *parent) : QObject(parent)
{

    Processa=newProcessa;
    IsConnesso=false;
    ServerIp="";
    host=newHost;
    Porta=newPorta;

    Client = new CThreadClient(Processa,host,Porta);
    connect(Client, SIGNAL(finished()), this, SLOT(onConnesioneInterrotta()));
    connect(Client, SIGNAL(Connesso(bool)),this, SLOT(onConnesso(bool)));
    connect(Client, SIGNAL(ConnesioneInterrotta()),this, SLOT(onConnesioneInterrotta()));
    Client->start();

    riconnettitimer = new QTimer(this);
    QObject::connect(riconnettitimer, SIGNAL(timeout()),
                     this, SLOT(RiConnetti()));

}


void QLabserverNet::onConnesioneInterrotta(){

     riconnettitimer->start(1000);
}





void QLabserverNet::onConnesso(bool val){

    if(!val){
        Processa->AzzeraNetData();
        riconnettitimer->start(1000);
    }else{
        emit Evento("PlcNet Connesso" );
        riconnettitimer->stop();
    }
    IsConnesso=val;


}


void QLabserverNet::RiConnetti()
{
     char *pErrore=Processa->pBufferErrore;
     if(pErrore==NULL)
        Processa->pBufferErrore=new char[Processa->ErrSize];

     Client = new CThreadClient(Processa,host,Porta);
     connect(Client, SIGNAL(finished()), this, SLOT(onDisconnesso()));
     connect(Client, SIGNAL(Connesso(bool)),this, SLOT(onConnesso(bool)));
     connect(Client, SIGNAL(ConnesioneInterrotta()),this, SLOT(onConnesioneInterrotta()));
     Client->start();


}







