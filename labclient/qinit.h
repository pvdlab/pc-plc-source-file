#ifndef QINIT_H
#define QINIT_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTimer>

class QInit : public QThread
{
    Q_OBJECT
public:
    explicit QInit(QWidget *parent,QTcpSocket *newPlcSocket);
    void run();

public slots:
    void PlcSocketLeggi();
    void PlcSoketConnesso();
    void PlcSoketDisconnesso();
    void PlcSocketError(QAbstractSocket::SocketError );
    void PlcSocketStateChange(QAbstractSocket::SocketState);
    void ScanPlc();

signals:
    void evento(QString );

public:
    QTcpSocket *PlcSocket;
    int         TipoApp;
    QHostAddress Plcsender;
    quint16 PlcsenderPort;

private:
    QStringList  ListaHostPlc;
    QTimer *timer;
    int index;

};

#endif // QINIT_H
