#include "qinit.h"
#include "qimpianto.h"

QInit::QInit(QWidget *parent,QTcpSocket *newPlcSocket)
{

   PlcSocket = newPlcSocket;

}

void QInit::run(){

    ListaHostPlc.append("192.168.4.3");
    ListaHostPlc.append("127.1.0.1");
    ListaHostPlc.append("192.168.0.113");
    ListaHostPlc.append("192.168.0.111");
    ListaHostPlc.append("192.168.0.102");
    ListaHostPlc.append("192.168.0.103");
    ListaHostPlc.append("192.168.0.104");
    ListaHostPlc.append("192.168.0.105");

    connect(PlcSocket, SIGNAL(readyRead()),
            this, SLOT(PlcSocketLeggi()));
    connect(PlcSocket, SIGNAL(connected()),
            this, SLOT(PlcSoketConnesso()));
    connect(PlcSocket, SIGNAL(disconnected()),
            this, SLOT(PlcSoketDisconnesso()));
    connect(PlcSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(PlcSocketError(QAbstractSocket::SocketError)));
    connect(PlcSocket,SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,SLOT(PlcSocketStateChange(QAbstractSocket::SocketState)));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(ScanPlc()));
    timer->start(500);

    index=0;

    exec();

    TipoApp=QTipoApp::PlcNet;
    PlcsenderPort=2000;
    for(int i=0 ; i<ListaHostPlc.count(); i++){
        QThread::msleep(5);
        QString str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
        str+=" Scan Plc to " + Plcsender.toString();
        emit evento(str);
        str=ListaHostPlc.at(i);
        Plcsender= QHostAddress(str);
        PlcSocket->disconnectFromHost();
        if(PlcSocket->state() == QAbstractSocket::ConnectedState){
           if (PlcSocket->waitForDisconnected(2000))
               qDebug("DiscoConnected!");
        }

        PlcSocket->connectToHost(Plcsender,PlcsenderPort);
        if (PlcSocket->waitForConnected(2000)){
            str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
            str+=" Connesso " + Plcsender.toString();
            emit evento(str);
            return;
        }
    }

    PlcSocket->disconnectFromHost();
    if(PlcSocket->state() == QAbstractSocket::ConnectedState){
       if (PlcSocket->waitForDisconnected(2000))
           qDebug("DiscoConnected!");
    }
    TipoApp=QTipoApp::ClienteNet;
    PlcsenderPort=4000;
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
        PlcsenderPort=4000;//ipPort;
        Plcsender=QHostAddress(ipAddress);
        QString str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
        str+=" Scan Remote Server " + Plcsender.toString();
        emit evento(str);
        PlcSocket->connectToHost(Plcsender,PlcsenderPort);
        if (PlcSocket->waitForConnected(20000)){
            str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
            str+=" Connesso " + Plcsender.toString();
            emit evento(str);

        }
    }

    QString str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    str+=" Nessuna Connessione" ;
    emit evento(str);

    exec();

}

void QInit::ScanPlc(){

    if(index==ListaHostPlc.count()+1)
        return;

    TipoApp=QTipoApp::PlcNet;
    PlcsenderPort=2000;
    if(index<ListaHostPlc.count()){
        QThread::msleep(5);
        QString str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
        str+=" Scan Plc to " + Plcsender.toString();
        emit evento(str);
        str=ListaHostPlc.at(index++);
        Plcsender= QHostAddress(str);
        PlcSocket->disconnectFromHost();
        if(PlcSocket->state() == QAbstractSocket::ConnectedState){
           if (PlcSocket->waitForDisconnected(2000))
               qDebug("DiscoConnected!");
        }

        PlcSocket->connectToHost(Plcsender,PlcsenderPort);
        if (PlcSocket->waitForConnected(2000)){
            str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
            str+=" Connesso " + Plcsender.toString();
            emit evento(str);
            timer->stop();
            return;
        }
        return;
    }

    PlcSocket->disconnectFromHost();
    if(PlcSocket->state() == QAbstractSocket::ConnectedState){
       if (PlcSocket->waitForDisconnected(2000))
           qDebug("DiscoConnected!");
    }
    TipoApp=QTipoApp::ClienteNet;
    PlcsenderPort=4000;
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
        PlcsenderPort=4000;//ipPort;
        Plcsender=QHostAddress(ipAddress);
        QString str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
        str+=" Scan Remote Server " + Plcsender.toString();
        emit evento(str);
        PlcSocket->connectToHost(Plcsender,PlcsenderPort);
        if (PlcSocket->waitForConnected(20000)){
            str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
            str+=" Connesso " + Plcsender.toString();
            emit evento(str);
            timer->stop();
            return;

        }
    }

    QString str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    str+=" Nessuna Connessione" ;
    emit evento(str);
    timer->stop();

}


void QInit::PlcSocketLeggi()
{
    QByteArray buffer ;

    while (PlcSocket->bytesAvailable() > 0){
        buffer.append(PlcSocket->readAll());
    }

    QString str;
    str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    str+=" Trovato to " + Plcsender.toString();

    emit evento(str);

}



void QInit::PlcSoketConnesso(){

    QImpianto Impianto;

    if(PlcSocket->state() == QAbstractSocket::ConnectedState){
       PlcSocket->write(Impianto.sendMsg(QMsg::GetData,0,0));
       qDebug() << Plcsender.toString();
    }

}

void QInit::PlcSoketDisconnesso(){

    //PlcSocket->connectToHost(Plcsender,PlcsenderPort;);

}


void QInit::PlcSocketError(QAbstractSocket::SocketError socketError){

    QString statestring;

    switch(socketError)
        {
        case 0:
             statestring= " CE01 Connessione rifiutata";
             break;
        case 2 :
             statestring= " CE02 Indirizzo non trovato";
             break;
        case QAbstractSocket::NetworkError:
             statestring= " CE03 Errore al netwark";
             break;
        case QAbstractSocket::RemoteHostClosedError:
             statestring=  "CE04 Server Disconnect";
             break;
        default:
             statestring= " CE05 ?? " ;
             qDebug() <<"error is ......." << socketError;
             break;
        }

    QString str;
    str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    str+= " " + Plcsender.toString() + " " + statestring;
    emit evento(str);

}

void QInit::PlcSocketStateChange(QAbstractSocket::SocketState socketState){
    QString statestring;

    switch(socketState)
    {
        case QAbstractSocket::UnconnectedState :
            statestring=" ST01 the socket is not connected";
            break;
        case QAbstractSocket::HostLookupState :
            statestring=" ST02 the socket is performing a host name lookup";
            break;
        case QAbstractSocket::ConnectingState :
            statestring=" ST03 the socket has started establishing a connection";
            break;
        case QAbstractSocket::ConnectedState :
            statestring=" ST04 a connection is established";
            break;
        case QAbstractSocket::BoundState :
            statestring=" ST05 the socket is bound to an address and port";
            break;
        case QAbstractSocket::ClosingState :
            statestring=" ST06 the socket is about to close";
            break;
        case QAbstractSocket::ListeningState :
            statestring="ST07 listening state";
            break;
        default:
            statestring="unknown state";
    }
    QString str;
    str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    str+= " " + Plcsender.toString() + " " + statestring;
    emit evento(str);
}
