#include "plcnet.h"
#include "qserver.h"
#include "ui_qmainplcnet.h"
#include <QtNetwork>
#include <QNetworkReply>
#include <QMessageBox>
#include <QTextStream>
#include <QDataStream>
#include <QNetworkAccessManager>
#include "../srcplc/StdAfx.h"
#include "../srcplc/Costanti.h"
#include "qnetthread.h"

#define PLC_NET_TIME 100
#define PLC_SEND_TIME 200



QMainPlcnet::QMainPlcnet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QMainplcnet)
{
    ui->setupUi(this);
    DaClient=false;

    QDesktopWidget desktop;

    int desktopHeight=desktop.geometry().height();
    int desktopWidth=desktop.geometry().width();
    this->resize(desktopWidth,desktopHeight);

    Connetti();
    //this->setWindowTitle("mio titolo");
}


QMainPlcnet::~QMainPlcnet()
{
    //netThread.terminate();
    delete ui;
}


void QMainPlcnet::Connetti(){

     //Plcsender=QHostAddress("88.149.249.5");
     Plcsender=QHostAddress("192.168.4.3");
     //Plcsender=QHostAddress("192.168.0.113");
     //Plcsender=QHostAddress::LocalHost;
     PlcsenderPort=2000;



     PlcSocket = new QUdpSocket(this);
     ClientSocket = new QTcpSocket(this);

     connect(&timeOut, SIGNAL(timeout()),
             this, SLOT(nessunaRisposta()));

     connect(PlcSocket, SIGNAL(readyRead()),
             this, SLOT(PlcSocketLeggi()));
     connect(PlcSocket, SIGNAL(connected()),
             this, SLOT(PlcSoketConnesso()));
     connect(PlcSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(PlcSocketError(QAbstractSocket::SocketError)));
     connect(PlcSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
             this,SLOT(PlcSocketStateChange(QAbstractSocket::SocketState)));

     connect(ClientSocket, SIGNAL(readyRead()),
             this, SLOT(ClientSocketLeggi()));
     connect(ClientSocket, SIGNAL(connected()),
             this, SLOT(ClientSoketConnesso()));
     connect(ClientSocket, SIGNAL(disconnected()),
             this, SLOT(ClientSoketDisconnesso()));
     connect(ClientSocket,SIGNAL(error(QAbstractSocket::SocketError)),
             this,SLOT(PlcSocketError(QAbstractSocket::SocketError)));
     connect(ClientSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
             this,SLOT(PlcSocketStateChange(QAbstractSocket::SocketState)));

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipEoloAddress = ipAddressesList.at(i);
            if(ipEoloAddress.toString().left(3)!="192"){
              ipEoloAddress=ipAddressesList.at(i);
              break;
            }
        }
    }

    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

     PlcSocket->connectToHost(Plcsender,PlcsenderPort);

    /* QNetThread netThread;
     connect(&netThread, SIGNAL(netEvento(QString)),
             this, SLOT(mostraEvento(QString)));
     connect(&netThread, SIGNAL(finished()),
             this, SLOT(deleteLater()));

     netThread.start();*/

     timeOut.setInterval(2000);


}

void QMainPlcnet:: mostraEvento(QString msg){

    ui->Impianto->mostraEvento(msg);
}



void QMainPlcnet::PlcSoketConnesso(){

    ui->Impianto->mostraEvento("PlcNet Connesso" );
    timerPlcNet.start(PLC_NET_TIME,this);
    PlcSocket->writeDatagram(ui->Impianto->sendMsg(QMsg::GetData,0,0), Plcsender, PlcsenderPort);

}

void QMainPlcnet::PlcSoketDisconnesso(){

    ui->Impianto->mostraEvento("PlcNet Provo a riconnetermi" );
    PlcSocket->connectToHost(Plcsender,PlcsenderPort);

}


void QMainPlcnet::PlcSocketError(QAbstractSocket::SocketError socketError){

    switch(socketError)
        {
        case 0:
             ui->Impianto->mostraEvento("CE01 Connection was refused"); //Connection was refused by the peer (or timed out). The host address was not found.
             break;
        case 2 :
             ui->Impianto->mostraEvento("CE02 Host address was not found");
             break;
        case QAbstractSocket::NetworkError:
             ui->Impianto->mostraEvento("CE03 Error occurred with the network");
             break;
        case QAbstractSocket::RemoteHostClosedError:
             ui->Impianto->mostraEvento("CE04 Server Disconnect");
             break;
        default:
             ui->Impianto->mostraEvento("CE05 ?? " );
             qDebug() <<"error is ......." << socketError;
             break;
        }

}

void QMainPlcnet::PlcSocketStateChange(QAbstractSocket::SocketState socketState){
    QString statestring;

    switch(socketState)
    {
        case QAbstractSocket::UnconnectedState : statestring="ST01 the socket is not connected";
            break;
        case QAbstractSocket::HostLookupState : statestring="ST02 the socket is performing a host name lookup";
            break;
        case QAbstractSocket::ConnectingState : statestring="ST03 the socket has started establishing a connection";
            break;
        case QAbstractSocket::ConnectedState : statestring="ST04 a connection is established";
            break;
        case QAbstractSocket::BoundState : statestring="ST05 the socket is bound to an address and port";
            break;
        case QAbstractSocket::ClosingState : statestring="ST06 the socket is about to close";
            break;
        case QAbstractSocket::ListeningState : statestring="ST07 listening state";
            break;
        default: statestring="unknown state";
    }
    ui->Impianto->mostraEvento( statestring);
}

void QMainPlcnet::PlcSocketLeggi()
{
    if(TipoApp!=QTipoApp::PlcNet){
       TipoApp=QTipoApp::PlcNet;
       plcSendTime=100;
       //if(ipEoloAddress.toString().left(3)!="192" ){
           QFile *SetFile;
           QTextStream *Out;
           SetFile = new QFile("/home/ricvb/Dropbox/indirizzo.dat");
           if (SetFile->open(QIODevice::WriteOnly | QIODevice::Text)){
               Out = new QTextStream(SetFile);
               *Out << ipEoloAddress.toString() << "\n";
               *Out << 4000 << "\n";
               SetFile->close();
           }
       //}
       timerPlcNet.stop();
       ui->Impianto->Connesso(TipoApp,Plcsender.toString());
       Server = new QServer(this,ui->Impianto);
       if (!Server->listen(QHostAddress::Any, 4000)) {
            QMessageBox::critical(this, tr("Threaded Fortune Server"),
                                  tr("Unable to start the server: %1.")
                                  .arg(Server->errorString()));
            close();
        }
       connect(Server, SIGNAL(NuovaConnessione(qintptr)),
               this, SLOT( NuovaConnessione(qintptr) ));
       timerSpedisci.start(plcSendTime,this);
    }

    QByteArray datagram;
    while (PlcSocket->hasPendingDatagrams()) {
        datagram.resize(PlcSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        PlcSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
    }

    riceviMsg(datagram);

}


void QMainPlcnet::on_Impianto_SpedisciaPlc(QByteArray msg){

    if(!ui->Impianto->VerificaMsg(msg)){
        ui->Impianto->mostraEvento("Non Spedisco Messaggio non valido" );
        return;
    }

    QMsgDaSpedire pMsg;
    pMsg.pMsg.append(msg);
    pMsg.Tentativi=0;

    msgDaSpedire.append(pMsg);

}


void QMainPlcnet::spedisciMsg(){

    QByteArray msg;
    if(msgDaSpedire.count()){
       QMsgDaSpedire pMsg=msgDaSpedire.at(0);
       if(pMsg.Tentativi>5){
          ui->Impianto->mostraEvento("Passati tre tentativi di spedizione" );
          ui->Impianto->RiceviErrore(pMsg.pMsg);
          msgDaSpedire.removeAt(0);
          return;
       }
       pMsg.Tentativi++;
       msgDaSpedire.replace(0,pMsg);
       msg=pMsg.pMsg;
    }else
       msg=ui->Impianto->sendMsg(QMsg::GetData,0,0);



    if(TipoApp==QTipoApp::PlcNet){
        if(PlcSocket->writeDatagram(msg, Plcsender, PlcsenderPort)==-1){
           ui->Impianto->mostraEvento("Errore in Spedizione" );
        }
    }
    else if(TipoApp==QTipoApp::ClienteNet){
        if(ClientSocket->state() == QAbstractSocket::ConnectedState){
           ClientSocket->write(msg);
        }
    }
    timeOut.start(5000);

}

void QMainPlcnet::riceviMsg(QByteArray rMsg){

    for(int i=0; i< msgDaSpedire.count();i++){
        QMsgDaSpedire pMsg=msgDaSpedire.at(i);
        QByteArray sMsg=pMsg.pMsg;
        BYTE *psMsg=sMsg.data();
        BYTE *prMsg=rMsg.data();
        if(*prMsg==*psMsg){
          msgDaSpedire.removeAt(i);
          break;
        }

    }
    timeOut.stop();

    if(!ui->Impianto->VerificaMsg(rMsg)){
        ui->Impianto->mostraEvento("Ricevo Messaggio non valido " );
        return;
    }

    if(TipoApp==QTipoApp::PlcNet)
      ui->Impianto->RiceviDaPlc(rMsg);
    else
      ui->Impianto->RiceviDaPlcNet(rMsg);

}

void QMainPlcnet::timerEvent(QTimerEvent *event){

    if (event->timerId() == timerPlcNet.timerId()) {
        if(Plcsender!= QHostAddress::LocalHost){
           Plcsender= QHostAddress::LocalHost;
           PlcSocket->disconnectFromHost();
           PlcSocket->connectToHost(Plcsender,PlcsenderPort);
           return;
        }
        //PlcSocket->disconnectFromHost();
        QFile *SetFile;
        QTextStream *Out;
        SetFile = new QFile("/home/ricvb/Dropbox/indirizzo.dat");
        QString ipAddress;
        int ipPort;
        if (SetFile->open(QIODevice::ReadOnly | QIODevice::Text)){
            Out = new QTextStream(SetFile);
            *Out >> ipAddress ;
            *Out >> ipPort ;
            SetFile->close();
            ClientsenderPort=4000;//ipPort;
            Clientsender=QHostAddress(ipAddress);
            ClientSocket->connectToHost(Clientsender,ClientsenderPort);
        }
        timerPlcNet.stop();

    }else if(event->timerId() == timerClientNet.timerId()){
        timerClientNet.stop();
    } else if(event->timerId() == timerSpedisci.timerId()){
        spedisciMsg();
    }
    else {
        QWidget::timerEvent(event);
    }
}

void QMainPlcnet::ClientSoketConnesso(){

    ClientSocket->write(ui->Impianto->sendMsg(QMsg::GetData,0,0));
    timerClientNet.start(PLC_NET_TIME,this);

}

void QMainPlcnet::ClientSoketDisconnesso(){


}

void QMainPlcnet::ClientSocketError(QAbstractSocket::SocketError socketError){

    switch(socketError)
        {
        case 0:QMessageBox::critical(0,"connection error","The connection was refused by the peer (or timed out).",QMessageBox::Ok);
               break;
        case 2:QMessageBox::critical(0,"connection error","The host address was not found.",QMessageBox::Ok);
               break;
        case QAbstractSocket::NetworkError:QMessageBox::critical(0,"connection error","An error occurred with the network .",QMessageBox::Ok);
               break;

        case QAbstractSocket::RemoteHostClosedError:
               QMessageBox::critical(0,"connection error","disconnect .",QMessageBox::Ok);
               break;
        default:QMessageBox::critical(0,"connection error","undefine error.",QMessageBox::Ok);
            qDebug() <<"error is ......."<< socketError;
            break;
        }

}

void QMainPlcnet::ClientSocketStateChange(QAbstractSocket::SocketState socketState){

    QString statestring;

    switch(socketState)
    {
        case QAbstractSocket::UnconnectedState : statestring="the socket is not connected";
            break;
        case QAbstractSocket::HostLookupState : statestring="the socket is performing a host name lookup";
            break;
        case QAbstractSocket::ConnectingState : statestring="the socket has started establishing a connection";
            break;
        case QAbstractSocket::ConnectedState : statestring="a connection is established";
            break;
        case QAbstractSocket::BoundState : statestring="the socket is bound to an address and port";
            break;
        case QAbstractSocket::ClosingState : statestring="the socket is about to close";
            break;
        case QAbstractSocket::ListeningState : statestring="listening state";
            break;
        default: statestring="unknown state";
    }

    QMessageBox::information(this, tr("ClientSocketError "),
                             statestring );
}

void QMainPlcnet::ClientSocketLeggi()
{
    if(TipoApp!=QTipoApp::ClienteNet){
      TipoApp=QTipoApp::ClienteNet;
      ui->Impianto->Connesso(TipoApp,Clientsender.toString());
      plcSendTime=300;
      timerSpedisci.start(plcSendTime,this);
    }

    QByteArray buffer ;

    while (ClientSocket->bytesAvailable() > 0){
        buffer.append(ClientSocket->readAll());
    }

    riceviMsg(buffer);


}

void QMainPlcnet::nessunaRisposta(){

    if(TipoApp!=QTipoApp::PlcNet){
       ui->Impianto->mostraEvento("PlcNet Time Out Mi Disconneto" );
       PlcSocket->disconnectFromHost();
    }else{
       ui->Impianto->mostraEvento("ClienteNet Time Out Mi Disconneto" );
       ClientSocket->disconnectFromHost();
    }

    timeOut.stop();

}
