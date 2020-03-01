#ifndef QMAINPLCNET_H
#define QMAINPLCNET_H

#include <QWidget>
#include "qserver.h"
#include <QTimer>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QNetworkReply>
#include "../srcplc/Processa.h"
#include  "qnetthread.h"

class QMsgDaSpedire
{
public:
    int Tentativi;
    QByteArray pMsg;

};

namespace Ui {
class QMainplcnet;
}

class QMainPlcnet : public QWidget
{
    Q_OBJECT

public:
    explicit QMainPlcnet(QWidget *parent = 0);
    ~QMainPlcnet();
    void Connetti();

protected:
    void timerEvent(QTimerEvent *timerEvent);

public slots:
    void PlcSocketLeggi();
    void PlcSoketConnesso();
    void PlcSoketDisconnesso();
    void PlcSocketError(QAbstractSocket::SocketError );
    void PlcSocketStateChange(QAbstractSocket::SocketState);
    void ClientSoketConnesso();
    void ClientSoketDisconnesso();
    void ClientSocketLeggi();
    void ClientSocketError(QAbstractSocket::SocketError );
    void ClientSocketStateChange(QAbstractSocket::SocketState);
    void mostraEvento(QString);
    void nessunaRisposta();


private slots:
    void on_Impianto_SpedisciaPlc(QByteArray msg);


private:
    void spedisciMsg();
    void riceviMsg(QByteArray msg);

private:
    QServer *Server;
    //QNetThread netThread;
    //int step,i,j;
    //int PlcSocketNoRisposta;
    QNetworkAccessManager *accessManager;
    QUdpSocket *PlcSocket;
    QTcpSocket *ClientSocket;
    QHostAddress Plcsender;
    quint16 PlcsenderPort;
    QHostAddress Clientsender;
    QHostAddress ipEoloAddress;
    int TipoApp;
    QBasicTimer timerPlcNet;
    QBasicTimer timerClientNet;
    QBasicTimer timerSpedisci;
    QTimer      timeOut;
    int         plcSendTime;
    QList<QMsgDaSpedire > msgDaSpedire;
    quint16 ClientsenderPort;
    bool DaClient;
    Ui::QMainplcnet *ui;


};

#endif // QMAINPLCNET_H
