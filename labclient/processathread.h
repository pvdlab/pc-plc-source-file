#ifndef PROCESSATHREAD_H
#define PROCESSATHREAD_H

#include "../srcplc/Processa.h"
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QByteArray>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QNetworkInterface>

class QProcessaThread: public QThread
{
    Q_OBJECT

public:
    QProcessaThread(QObject *parent = 0);
    ~QProcessaThread();
    void run();
    void Comando(QByteArray );

signals:
    void Risposta(QByteArray );
    void MainTimer(long);
    void netEvento(QString msg);

private:
    bool isConnectedToNetwork();
    CProcessa *Processa;
    QMutex mutex;
    QWaitCondition cond;
    bool quit;
    bool isconnesso;

};

#endif // PROCESSATHREAD_H
