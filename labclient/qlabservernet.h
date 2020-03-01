#ifndef QLABSERVERNET_H
#define QLABSERVERNET_H

#include <QTimer>
#include <QObject>
#include "NetPlc.h"
#include "cthreadclient.h"

class QLabserverNet : public QObject
{
    Q_OBJECT
public:
    explicit QLabserverNet(QNetPlc     *NewProcessa,QString newHost, int newPorta,QObject *parent = 0);

    bool          IsConnesso;
    quint16       ServerPort;
    QString       ServerIp;
    CThreadClient *Client;


signals:
    void Evento(QString);
    void ProcessoCaricato();

public slots:
    void onConnesso(bool);
    void RiConnetti();
    void onConnesioneInterrotta();


private:
    QNetPlc       *Processa;
    QTimer        *riconnettitimer;
    QString       host;
    int           Porta;

};

#endif // QLABSERVERNET_H
